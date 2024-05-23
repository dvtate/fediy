#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <algorithm>

#include "globals.hpp"
#include "../util/RWMutex.hpp"

#include "Peer.hpp"
#include "User.hpp"


class Cache {
    std::unordered_map<std::string, std::shared_ptr<Peer>> m_peers;
    RWMutex m_peers_mtx;    
    std::unordered_map<std::string, std::shared_ptr<User>> m_users;
    RWMutex m_users_mtx;


    void load_peers_from_db();

    void prune();


    template<class... Args>
    std::shared_ptr<Peer> add_peer(std::string domain, Args&&... args) {
        m_peers.emplace(std::move(domain), ...args);
    }
    std::shared_ptr<Peer> get_peer(const std::string& domain);

    template<class... Args>
    std::shared_ptr<Peer> add_user(std::string username, Args&&... args) {
        m_peers.emplace(std::move(username), ...args);
    }
    std::shared_ptr<Peer> get_user(const std::string& username);

};
