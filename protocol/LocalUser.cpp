#include "LocalUser.hpp"

#include "App.hpp"

std::string LocalUser::login(const std::string& username, const std::string& password) {
    // Loop in extremely rare event of token overlap
    auto user = g_app->m_db->get_user(username, password);
    if (!user)
        return "";
    for (;;) {
        auto p = g_app->m_cache.add_user(LocalUser::AuthToken(user));
        // Give the token if successful
        if (p.second)
            return p.first->first;

        // else: token collision (rare)
    }
}