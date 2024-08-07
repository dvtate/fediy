//
// Created by tate on 8/6/24.
//

#include "../modlib/fediymod.h"
#include "MailBox.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <set>

const fediy::fiy_host_info_t* g_host_info;

MailBox g_mailbox;

std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.emplace_back(str.substr(prev, pos - prev));
        prev = pos + delimiter.size();
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.emplace_back(str.substr(prev));

    return strings;
}

std::string user_str(const fediy::fiy_request_t* request) {
    std::string ret = request->user;
    ret += '@';
    ret += request->domain != nullptr ? request->domain : g_host_info->domain;
    return ret;
}

void send_mail(fediy::fiy_request_t* request, fediy::fiy_callback_t callback) {
    if (request->user == nullptr) {
        // Unauthenticated
        fediy::fiy_response_t resp { .status=401, .body="Unauthenticated" };
        callback(request, &resp);
        return;
    }

    std::string body = request->body;
    auto i = body.find('\n');
    if (i == std::string::npos) {
        fediy::fiy_response_t resp { .status=400 };
        callback(request, &resp);
        return;
    }
    std::string to_str = body.substr(0, i);
    auto to = split_string(to_str, ",");
    auto old_i = i + 1;
    i = body.find('\n', old_i);
    if (i == std::string::npos) {
        fediy::fiy_response_t resp { .status=400 };
        callback(request, &resp);
        return;
    }
    std::string subject = body.substr(old_i, i - old_i);
    std::string local_dom = "@";
    local_dom += g_host_info->domain;
    for (auto& recip: to)
        if (recip.find('@') == std::string::npos)
            recip += local_dom;

    std::string content = body.substr(i);
    std::cout <<"new mail: " <<user_str(request) <<" : " <<to[0] <<" : " <<subject <<" : " <<content<<std::endl;
    g_mailbox.push(Mail(user_str(request), to, subject, content));

    // Distribute to peers
    if (request->domain == nullptr) {
        // Get unique remote destinations
        std::set<std::string> doms;
        for (const auto& user : to) {
            auto i = user.find('@');
            if (i != std::string::npos) {
                doms.emplace(user.substr(i + 1));
            }
        }
        doms.erase(g_host_info->domain);

        // Send to destination servers
        for (const auto& dom : doms) {
            request->domain = dom.c_str();
            std::cout <<"sending to: " <<dom <<std::endl;
            g_host_info->request("mail", request, nullptr);
        }
    }
    fediy::fiy_response_t resp = {.status=200, .body="sent"};
    callback(request, &resp);
}

const char* compose_html = "<!doctype html><html><body>\n"
                           "<form action='javascript:submit()'>\n"
                           "To: <input type='text' id='inp-to' placeholder='tate@dvtt.net,test@example.com,jerry' />\n"
                           "<br/>Subject: <input type='text' id='inp-subject' placeholder='Important message' />\n"
                           "<br/>Body: <textarea id='inp-body'>Write your email here</textarea>\n"
                           "<br/><button type='submit'>Send</button>\n"
                           "</form>\n"
                           "<script>\n"
                           "async function submit() {\n"
                           "    fetch('send', {\n"
                           "            method: 'POST',\n"
                           "            body: document.getElementById('inp-to').value\n"
                           "                  + '\\n' + document.getElementById('inp-subject').value\n"
                           "                  + '\\n' + document.getElementById('inp-body').value\n"
                           "    }).then(v => window.location = 'outbox')\n"
                           "    .catch(console.error);\n"
                           "}\n"
                           "</script>\n"
                           "</body></html>";

static void handle_request(fediy::fiy_request_t* request, fediy::fiy_callback_t callback) {
    std::cout <<"Path: "<<request->path <<std::endl;
    if (strcmp(request->path, "/send") == 0) {
        send_mail(request, callback);
        return;
    } else if (strcmp(request->path, "/") == 0) {
        static const std::string root = g_host_info->base_uri;
        static const std::string body = "<ul>"
                           "<li><a href='" + root + "/inbox'>Inbox</a></li>"
                           "<li><a href='" + root + "/outbox'>Outbox</a></li>"
                           "<li><a href='" + root + "/compose'>Compose</a></li>"
                           "</ul>";
        fediy::fiy_response_t resp = {
            .status=200,
            .body=body.c_str()
        };
        callback(request, &resp);
        return;
    } else if (strcmp(request->path, "/inbox") == 0) {
        if (request->domain == nullptr && request->user != nullptr) {
            auto inbox_str = g_mailbox.get_inbox_str(user_str(request));
            fediy::fiy_response_t resp = { .status=200, .body=inbox_str.c_str() };
            callback(request, &resp);
            return;
        }
        // Unauthenticated
        fediy::fiy_response_t resp { .status=401, .body="Unauthenticated" };
        callback(request, &resp);
        return;
    } else if (strcmp(request->path, "/outbox") == 0) {
        if (request->domain == nullptr && request->user != nullptr) {
            auto outbox_str = g_mailbox.get_outbox_str(user_str(request));
            fediy::fiy_response_t resp = { .status=200, .body=outbox_str.c_str() };
            callback(request, &resp);
            return;
        }
        // Unauthenticated
        fediy::fiy_response_t resp { .status=401, .body="Unauthenticated" };
        callback(request, &resp);
        return;

    } else if (strcmp(request->path, "/compose") == 0) {
        fediy::fiy_response_t resp {.status=200, .body=compose_html };
        callback(request, &resp);
        return;
    } else if (strncmp(request->path, "/view/", strlen("/view/")) == 0) {
        auto idx = strtoul(request->path + strlen("/view/"), nullptr, 10);
        // TODO verify that the user has access to the email
        auto* m = g_mailbox.get(idx);
        if (m == nullptr) {
            fediy::fiy_response_t resp = {.status=404, .body="not found"};
            callback(request, &resp);
            return;
        }
        auto view = m->long_view();
        fediy::fiy_response_t resp = {.status=200, .body=view.c_str()};
        callback(request, &resp);
        return;
    }

    std::string body = "Hello, @";
    if (request->user != nullptr)
        body += request->user;
    body += "@";
    if (request->domain != nullptr)
        body += request->domain;
    body += "! <br/>Path: ";
//    body += request->method;
    body += " ";
    if (request->path != nullptr)
        body += request->path;

    fediy::fiy_response_t r={
            .status=404,
            .body=body.c_str()
    };
    callback(request, &r);
}

//
//// if domain is null, then user is local
//void (* username_change_handler)(const char* domain, const char* old_username, const char* new_username);
//
///// peer domain changed
//void (* peer_domain_change_handler)(const char* old_domain, const char* new_domain);
//

extern "C" fediy::fiy_mod_info_t* start(const fediy::fiy_host_info_t* host_info) {
    static fediy::fiy_mod_info_t mod_info = {
            .on_request=handle_request,
            .on_peer_domain_changed=nullptr,
            .on_username_changed=nullptr,
    };
    g_host_info = host_info;
    return &mod_info;
}
