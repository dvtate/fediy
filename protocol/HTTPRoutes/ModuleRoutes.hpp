//
// Created by tate on 6/25/24.
//

#pragma once


#include "drogon/HttpController.h"

#include "drogon/utils/Utilities.h"

class ModuleRoutes : public drogon::HttpController<ModuleRoutes, false> {
public:

    struct User {
        const char* domain{nullptr};
        std::string user;
    };

    static User find_user(const drogon::HttpRequestPtr& req);

    void app_redirect(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void app_send_msg(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ModuleRoutes::app_redirect, "/mods/{mod_id}", drogon::Get);
        ADD_METHOD_VIA_REGEX(ModuleRoutes::app_send_msg, ".*",
             drogon::Get, drogon::Post, drogon::Delete, drogon::Put, drogon::Patch, drogon::Head);

    METHOD_LIST_END
};
