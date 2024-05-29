#include <openssl/sha.h>

#include "Auth.hpp"

#include "App.hpp"


bool init() {
    // Set public and private keys
    std::string path = g_app->m_config.m_data_dir + "/pubkey";

    return true;
}


std::string Auth::auth_local_user(const std::string& username, const std::string& password) {
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
        bool success = g_app->m_cache.add_user(token, ret).second;
        if (success)
            return ret;
    }
}

