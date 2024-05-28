#pragma once

#include <string>
#include <memory>
#include <deque>
#include <unordered_map>

#include "Peer.hpp"

// dtesta: is this even getting used?
class LocalUser {
public:
    std::string m_username; // name@example.com

    LocalUser(std::string username, std::string bearer_token, time_t token_expiration_ts):
        m_username(std::move(username)),
        m_bearer_token(std::move(bearer_token)),
        m_token_expiration_ts(token_expiration_ts)
    {}

    bool is_auth_expired(const time_t now = std::time(0)) {
        return now > m_token_expiration_ts;
    }
};
