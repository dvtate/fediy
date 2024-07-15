//
// Created by tate on 6/25/24.
//

#pragma once


#include "drogon/HttpController.h"

#include "drogon/utils/Utilities.h"


class ModuleRoutes {
public:

    struct User {
        const char* domain{nullptr};
        std::string user;
    };

    static User find_user(const drogon::HttpRequestPtr& req);
};
