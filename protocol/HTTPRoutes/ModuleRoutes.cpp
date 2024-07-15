//
// Created by tate on 6/25/24.
//

#include "ModuleRoutes.hpp"

#include "LocalUser.hpp"

ModuleRoutes::User ModuleRoutes::find_user(const drogon::HttpRequestPtr& req) {
    auto u = req->session()->getOptional<std::shared_ptr<LocalUser>>("user").value_or(nullptr);
    if (u != nullptr)
        return { .domain=nullptr, .user=u->get_username() };

    // Peer authentication
}