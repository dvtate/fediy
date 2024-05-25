#pragma once

#include <unordered_map>
#include <random>
#include <string>
#include <list>

#include "globals.hpp"
#include "../util/RWMutex.hpp"

#include "LocalUser.hpp"
#include "Peer.hpp"

class Auth {

    static constexpr time_t USER_TOKEN_LIFETIME = 60 * 60 * 24 * 14; // 2 weeks
    static constexpr time_t PEER_TOKEN_LIEFTIME = 60 * 60 * 24 * 7; // 1 week

    static std::string get_auth_token() {
        // Create random generator that picks indicies charset
        // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
        static thread_local std::random_device rd;
        static thread_local std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned short> dist(1, 63);
        const char charset[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

        // Generate token using random chars from charset
        // Probably a better way to do this that doesn't use operator+=
        std::string ret;
        ret.reserve(24);
        for (int i = 0; i < 24; i++)
            ret += charset[dist(gen)];

        return ret;
    }

public:

    std::shared_ptr<LocalUser> auth_local_user(const std::string& username, const std::string& password);

};