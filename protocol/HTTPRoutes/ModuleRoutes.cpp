//
// Created by tate on 6/25/24.
//

#include "ModuleRoutes.hpp"

#include "LocalUser.hpp"
#include "App.hpp"

ModuleRoutes::User ModuleRoutes::find_user(const drogon::HttpRequestPtr& req) {
    // Local User authentication
    auto u = req->session()
        ->getOptional<std::shared_ptr<LocalUser>>("user").value_or(nullptr);
    if (u != nullptr)
        return { .domain=nullptr, .user=u->get_username() };

    // Peer authentication
    // TODO

    // No authentication
    return { .domain=nullptr, .user="" };
}

void ModuleRoutes::app_redirect(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    const auto m = g_app->m_mods.get_mod_by_id(req->getRoutingParameters()[0]);
    if (m == nullptr) {
        static auto ret404 = drogon::HttpResponse::newNotFoundResponse();
        callback(ret404);
    } else {
//        std::cout <<"redirecting to /" <<m->m_path <<std::endl;
        callback(drogon::HttpResponse::newRedirectionResponse("/" + m->m_path));
    }
}

void ModuleRoutes::app_send_msg(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    const auto slash_idx = req->path().find('/', 1);
    std::cout <<slash_idx <<std::endl;
    std::string app, uri;
    if (slash_idx == -1) {
        app = req->path().data() + 1;
        uri = "/";
    } else {
        app = req->path().substr(1, slash_idx - 1);
        uri = req->path().data() + slash_idx;
    }
//    std::cout <<"Calling " <<app <<" : " << uri <<std::endl;
    g_app->m_mods.get_mod_by_path(app)->m_ipc->handle_request(req, find_user(req), std::move(callback));
}
