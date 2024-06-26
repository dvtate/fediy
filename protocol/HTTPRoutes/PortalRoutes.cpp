//
// Created by tate on 6/20/24.
//

#include "../App.hpp"

#include "drogon/utils/Utilities.h"

#include "PortalRoutes.hpp"


// parse application/x-www-form-urlencoded
bool parse_form_url_encoded(const std::string_view& s, std::deque<std::pair<std::string, std::string>>& ret) {
    size_t i = 0;
    do {
        size_t start = i;
        std::pair<std::string, std::string> kv;
        while (i < s.size() && s[i] != '=')
            i++;
        if (i == s.size() || s[i] != '=') {
            DEBUG_LOG("bad 1");
            return false;
        }
        auto csubstr = s.data();
        kv.first = drogon::utils::urlDecode(csubstr + start, csubstr + i);
        if (kv.first.empty()) {
            DEBUG_LOG("bad 2");
            return false;
        }

        start = ++i;
        while (i < s.size() && s[i] != '&')
            i++;
//        if (i == s.size() || s[i] != '&') {
//            DEBUG_LOG("bad 3");
//            return true;
//        }
        csubstr = s.data();
        kv.second = drogon::utils::urlDecode(csubstr + start, csubstr + i - 1);
        ret.emplace_back(std::move(kv));
        i++;

    } while (i < s.size());
    return true;
}

inline std::shared_ptr<LocalUser> get_user_from_session(const drogon::HttpRequestPtr& req) {
    return req->session()->getOptional<std::shared_ptr<LocalUser>>("user").value_or(nullptr);
}


drogon::HttpResponsePtr login_page_response(
    const drogon::HttpStatusCode status = drogon::HttpStatusCode::k200OK,
    const std::string& err = ""
) {
    auto ret = drogon::HttpResponse::newHttpResponse(status, drogon::CT_TEXT_HTML);
    ret->setBody(g_app->m_pages->login_page(err));
    return ret;
}

void PortalRoutes::login_get(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    static const auto resp_get = login_page_response();
    callback(resp_get);
}

void PortalRoutes::login_post(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    // Pre-allocated responses
    static const auto resp_get = login_page_response();
    static const auto resp_bad_form = login_page_response(drogon::HttpStatusCode::k400BadRequest, "Invalid Form");
    static const auto resp_bad_creds = login_page_response(drogon::HttpStatusCode::k401Unauthorized, "Incorrect username/password");

    if (req->method() == drogon::Get) {
        callback(resp_get);
        return;
    }

    // Get username and password from body
    std::deque<std::pair<std::string, std::string>> form;
    if (!parse_form_url_encoded(req->body(), form)) {
        callback(resp_bad_form);
        DEBUG_LOG("invalid body: '" <<req->bodyData() <<"'");
        return;
    }
    std::string username, password;
    for (auto&& [k, v]: std::move(form)) {
        if (k == "username")
            username = std::move(v);
        else if (k == "password")
            password = std::move(v);
        else {
            DEBUG_LOG("invalid form field: " << k );
        }
    }

    // root user?
//    if (username.empty()) {
//        callback(m_response_invalid_body);
//        DEBUG_LOG("empty username");
//        return;
//    }
//    if (password.empty()) {
//        callback(resp_bad_form);
//        DEBUG_LOG("empty password");
//        return;
//    }

    // Log user in
    auto user = g_app->m_db->get_user(username, password);
    if (!user) {
        callback(resp_bad_creds);
        DEBUG_LOG("wrong username/password for user " <<username);
        return;
    }
    req->session()->insert("user", user);

    // Send authenticated user to portal
    static const auto redirect_portal = drogon::HttpResponse::newRedirectionResponse("/portal");
    callback(redirect_portal);
}