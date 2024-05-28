#include <openssl/sha.h>

#include "Auth.hpp"

#include "App.hpp"



bool check_user_password()


std::shared_ptr<LocalUser> Auth::auth_local_user(const std::string& username, const std::string& password) {
    // Make sure hash of password matches what's in database
    unsigned char hashed_password[129];
    unsigned char* hp = SHA512((unsigned char*) password.c_str(), password.size(), hashed_password);
    auto db_pw = g_app->m_db.get_user_password(username);
    for (int i = 0 ; i < 128; i++) {
        if ((char)hp[i] != db_pw[i])
            return nullptr;
    }

    // Loop in extremely rare event of token overlap
    for (;;) {
        auto token = Auth::get_auth_token();
        auto ret = std::make_shared<LocalUser>(
                username,
                token,
                std::time(nullptr) + USER_TOKEN_LIFETIME);
        bool success = g_app->m_cache.add_user(token, ret).second;
        if (success)
            return ret;
    }
}

