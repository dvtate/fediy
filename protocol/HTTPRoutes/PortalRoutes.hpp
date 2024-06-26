//
// Created by tate on 6/20/24.
//

#pragma once

#include "drogon/HttpController.h"


class PortalRoutes : public drogon::HttpController<PortalRoutes, false> {
public:

    PortalRoutes() {}

    void login_get(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void login_post(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void signup_get(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void signup_post(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void home_get(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void main_js(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
//    void main_css(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
//    void settings_get(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
//    void settings_post(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);


    METHOD_LIST_BEGIN
        ADD_METHOD_TO(PortalRoutes::login_get, "/portal/login", drogon::HttpMethod::Get);
        ADD_METHOD_TO(PortalRoutes::login_post, "/portal/login", drogon::HttpMethod::Post);
        ADD_METHOD_TO(PortalRoutes::signup_get, "/portal/signup", drogon::HttpMethod::Get);
        ADD_METHOD_TO(PortalRoutes::signup_post, "/portal/signup", drogon::HttpMethod::Post); // TODO this should be via invite code only
        ADD_METHOD_TO(PortalRoutes::main_js, "/portal/main.js", drogon::HttpMethod::Get);
        ADD_METHOD_TO(PortalRoutes::home_get, "/portal", drogon::HttpMethod::Get);

//        ADD_METHOD_TO(PortalRoutes::settings_get, "/portal/settings", drogon::HttpMethod::Get);
//        ADD_METHOD_TO(PortalRoutes::settings_post, "/portal/settings", drogon::HttpMethod::Post);
//        ADD_METHOD_TO(PortalRoutes::main_css, "/portal/main.css", drogon::Get);
    METHOD_LIST_END
};
