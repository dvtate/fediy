#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <algorithm>
#include <variant>

#include "globals.hpp"
#include "../util/RWMutex.hpp"

#include "Peer.hpp"
#include "LocalUser.hpp"


class Cache {
    // domain -> peer
    std::unordered_map<std::string, std::shared_ptr<Peer>> m_peers;
    RWMutex m_peers_mtx;

    // bearer token -> user
    std::unordered_map<std::string, std::shared_ptr<LocalUser>> m_local_users; // this shouldn't need to be a shared_ptr
    RWMutex m_users_mtx;

    // bearer token -> peer
    std::unordered_map<std::string, std::shared_ptr<Peer>> m_peer_domains;

public:
//    void load_peers_from_db();

    void prune();

    template<class... Args>
    auto add_peer(std::string domain, Args&&... args) -> auto {
        RWMutex::LockForWrite lock{m_peers_mtx};
        return m_peers.emplace(domain, args...);
    }
    std::shared_ptr<Peer> get_peer(const std::string& domain);

    template<class... Args>
    auto add_user(std::string token, Args&&... args) -> auto {
        RWMutex::LockForWrite lock{m_users_mtx};
        return m_local_users.emplace(token, args...);
    }
    std::shared_ptr<LocalUser> get_user(const std::string& username);

};
