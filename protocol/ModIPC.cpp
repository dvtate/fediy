//
// Created by tate on 7/5/24.
//

#include "ModIPC.hpp"

#include "LocalUser.hpp"
#include "App.hpp"

static char* new_cstr_from_string(const std::string_view& s) {
    char* ret = new char[s.size() + 1];
    strncpy(ret, s.data(), s.size());
    return ret;
}

ModDllIpcRequest::ModDllIpcRequest(
        const drogon::HttpRequestPtr& req,
        ModuleRoutes::User&& user,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
): m_user(user), m_callback(callback) {
    // Initialize fiy_request_t
    this->method = req->methodString();
    this->body = new_cstr_from_string(req->body());
    this->path = new_cstr_from_string(req->path());
    this->domain = m_user.domain;
    this->user = m_user.user.c_str();
    this->headers = nullptr;
}


void ModDLLIPC::gen_host_info() {
    if (m_host_info != nullptr)
        free_host_info();

    m_host_info = new fediy::fiy_host_info_t;
    m_host_info->log = [](int n, const char* s){
        std::cout <<s <<std::endl;
    };
    std::string base_uri = (g_app->m_config.m_ssl ? "https://" : "http://")
                           + g_app->m_config.m_hostname + "/" + m_mod->m_id;
    char* cstr = new char[base_uri.size() + 1];
    strncpy(cstr, base_uri.c_str(), base_uri.size());
    m_host_info->base_uri = cstr;
}

void ModDLLIPC::handle_request(
    const drogon::HttpRequestPtr& req,
    ModuleRoutes::User&& user,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    auto* r = new ModDllIpcRequest(req, std::move(user), std::move(callback));
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