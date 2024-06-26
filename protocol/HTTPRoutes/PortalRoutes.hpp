//
// Created by tate on 6/20/24.
//

#pragma once

#include "drogon/HttpController.h"


class PortalRoutes : public drogon::HttpController<PortalRoutes, false> {
public:

    PortalRoutes() {}

    void login_post(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void login_get(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    METHOD_LIST_BEGIN
        ADD_METHOD_TO(PortalRoutes::login_get, "/", drogon::Get);
        ADD_METHOD_TO(PortalRoutes::login_get, "/portal/login", drogon::Get);
        ADD_METHOD_TO(PortalRoutes::login_post, "/portal/login", drogon::Post);
    METHOD_LIST_END
};
