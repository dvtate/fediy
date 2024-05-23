#pragma once

#include <string>
#include <memory>
#include <deque>
#include <unordered_map>

#include "Peer.hpp"

// dtesta: is this even getting used?
class User {
public:
    std::shared_ptr<Peer> m_peer;
    std::string m_username; // name@example.com
    std::string m_bearer_token;
    time_t m_bearer_token_expiration_ts;
};
