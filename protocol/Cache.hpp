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

    // bearer token -> user auth
    std::unordered_map<std::string, LocalUser::AuthToken> m_local_users;
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

    // TODO forward arguments to constructor
//    auto add_user(const LocalUser::AuthToken& token) -> auto {
//        RWMutex::LockForWrite lock{m_users_mtx};
//        return m_local_users.emplace(token.m_token, token);
//    }
//    std::shared_ptr<LocalUser> get_user_from_token(const std::string& token);

};
