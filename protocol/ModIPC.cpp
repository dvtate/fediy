//
// Created by tate on 7/5/24.
//

#include "ModIPC.hpp"

#include "LocalUser.hpp"
#include "App.hpp"

static char* new_cstr_from_string(const std::string_view& s) {
    auto l = s.size();
    char* ret = new char[l + 1];
    strncpy(ret, s.data(), l + 1);
    return ret;
}

ModDllIpcRequest::ModDllIpcRequest(
        const drogon::HttpRequestPtr& req,
        ModuleRoutes::User&& user,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
): m_user(user), m_callback(callback) {
    // Initialize fiy_request_t
    this->method = req->methodString();
    this->path = new_cstr_from_string(req->path());
    this->body = new_cstr_from_string(req->body());
    this->domain = m_user.domain;
    this->user = m_user.user == " " ? nullptr : m_user.user.c_str();
    this->headers = nullptr;
}

drogon::HttpMethod drogon_http_method(const std::string& method) {
    if (method == "GET")
        return drogon::HttpMethod::Get;
    if (method == "POST")
        return drogon::HttpMethod::Post;
    if (method == "PATCH")
        return drogon::HttpMethod::Patch;
    if (method == "PUT")
        return drogon::HttpMethod::Put;
    if (method == "DELETE")
        return drogon::HttpMethod::Delete;
    if (method == "HEAD")
        return drogon::HttpMethod::Head;
    if (method == "OPTIONS")
        return drogon::HttpMethod::Options;
    return drogon::HttpMethod::Invalid;
}

void send_request_to_app(
//    const struct fiy_host_info_t* host,
    const char* app_id,
    const fediy::fiy_request_t* request,
    void (*callback)(const fediy::fiy_response_t*)
) {
    // Convert request to drogon request
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setPath(request->path);
    req->setBody(request->body);
    req->setMethod(drogon_http_method(request->method));

    // Send request to peer
    g_app->m_peers.request_peer(
        request->domain, app_id, request->user, req,
        [callback](const drogon::HttpResponsePtr& resp) {
            std::cout <<"request_peer gave" <<resp->statusCode() <<std::endl;

            // Convert response and send it back to mod
            if (callback == nullptr)
                return;
            fediy::fiy_response_t r {
                .status = resp->getStatusCode(),
                .body = resp->body().data()
            };
            callback(&r);
        }
    );
}

void ModDLLIPC::gen_host_info() {
    if (m_host_info != nullptr)
        free_host_info();

    m_host_info = new fediy::fiy_host_info_t;
    m_host_info->log = [](int n, const char* s){
        std::cout <<s <<std::endl;
    };
    std::string base_uri = std::string(g_app->m_config.m_ssl ? "https://" : "http://")
            + g_app->m_config.m_hostname + '/' + m_mod->m_path;

    char* cstr = new char[base_uri.size() + 1];
    strcpy(cstr, base_uri.c_str());
    m_host_info->base_uri = cstr;
    m_host_info->request = send_request_to_app;
    m_host_info->domain = g_app->m_config.m_hostname;
}

void ModDLLIPC::handle_request(
    const drogon::HttpRequestPtr& req,
    ModuleRoutes::User&& user,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    fediy::fiy_request_t* r = new ModDllIpcRequest(req, std::move(user), std::move(callback));
    // everything good up to here
    m_mod_info->on_request(
        r,
        [](
            const fediy::fiy_request_t* req,
            const fediy::fiy_response_t* resp
        ){
            ((ModDllIpcRequest*) req)->callback(resp);
        }
    );
}

bool ModNetIPC::start() {

    // if server uri is localhost/127.0.0.1 then start the server

    // Assume the other server is already running
    // send hostinfo as json
    // expect modinfo as json

    // hostinfo should also include authentication system
    // ie - credentials we can validate for every request
    return true;
}

bool ModNetIPC::stop() {
    return true;
}