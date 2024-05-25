#include "Auth.hpp"

#include "App.hpp"


std::shared_ptr<LocalUser> Auth::auth_local_user(const std::string& username, const std::string& password) {
    // Make sure hash of password matches what's in database
    if (password != g_app->m_db.get_user_password(username))
        return nullptr;

    // Loop in extremely rare event of token overlap
    for (;;) {
        auto token = Auth::get_auth_token();
        auto ret = std::make_shared<LocalUser>(username, token, std::time(0) + USER_TOKEN_LIFETIME);
        bool success = g_app->m_cache.add_user(token, ret).second;
        if (success)
            return ret;
    }
}
