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


class Peers {
    RWMutex m_mtx;

    // domain -> peer
    std::unordered_map<std::string, std::shared_ptr<Peer>> m_peers_out;

    // bearer token -> peer
    std::unordered_map<std::string, std::shared_ptr<Peer>> m_peers_in;

public:
//    void load_peers_from_db();

    void prune();

    bool add_peer(const std::string& domain, const std::shared_ptr<Peer>& p) {
        RWMutex::LockForWrite lock{m_mtx};
        auto ret = m_peers_out.emplace(domain, p);
        if (!ret.second)
            return false;
        m_peers_in.emplace(p->m_auth.m_bearer_token_we_accept, p);
        return true;
    }

    std::shared_ptr<Peer> get_peer_for_domain(const std::string& domain);
    std::shared_ptr<Peer> get_peer_from_token(const std::string& domain);

    // TODO forward arguments to constructor
//    auto add_user(const LocalUser::AuthToken& token) -> auto {
//        RWMutex::LockForWrite lock{m_users_mtx};
//        return m_local_users.emplace(token.m_token, token);
//    }
//    std::shared_ptr<LocalUser> get_user_from_token(const std::string& token);
};
