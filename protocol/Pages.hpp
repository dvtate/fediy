//
// Created by tate on 6/4/24.
//

#pragma once

#include <string>

#include "../third_party/Mustache/mustache.hpp"

// TODO i18n
// TODO minification

class Pages {
    kainjow::mustache::mustache
        m_portal_apps_template,
        m_portal_settings_template,
        m_login_template,
        m_signup_template;

public:
    Pages();

    std::string login_page(const std::string& fail_reason = "");
    std::string signup_page(const std::string& fail_reason = "");

    std::string portal_apps(LocalUser& user);
    std::string portal_settings(LocalUser& user);

};
