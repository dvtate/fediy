//
// Created by tate on 7/5/24.
//

#include "ModIPC.hpp"

#include "LocalUser.hpp"
#include "App.hpp"

ModDllIpcRequest::ModDllIpcRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
): m_callback(callback) {
    this->body = new_cstr_from_string(req->body());
    this->path = new_cstr_from_string(req->path());
    this->domain = nullptr; // TODO
    auto user = req->session()->getOptional<std::shared_ptr<LocalUser>>("user").value_or(nullptr);
    if (user) {
        this->user = new_cstr_from_string(user->get_username());
    }
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
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    auto* r = new ModDllIpcRequest(req, std::move(callback));
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

