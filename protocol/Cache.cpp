#include "Cache.hpp"

#include "App.hpp"


/**
 * \returns null when not a peer invalid user token
 */
std::shared_ptr<Peer> Cache::get_peer(const std::string& domain) {
    m_peers_mtx.read_lock();

    auto it = m_peers.find(domain);

    if (it != m_peers.end()) {
        m_peers_mtx.read_unlock();
        return it->second;
    }
    m_peers_mtx.read_unlock();
//    return g_app->add_peer(domain);
    return nullptr;
}

/**
 * \returns null when invalid user token
 */
//std::shared_ptr<LocalUser> Cache::get_user_from_token(const std::string& token) {
//    m_users_mtx.read_lock();
//
//    auto it = m_local_users.find(token);
//    if (it != m_local_users.end()) {
//        m_users_mtx.read_unlock();
//        return it->second.m_user;
//    }
//
//    // Probably invalid token
//    return nullptr;
//}

void Cache::prune() {
    // Erase users with expired authentication
//    m_users_mtx.read_lock();
//    auto it_tok_end = m_user_tokens.begin();
//    for (; it_tok_end != m_user_tokens.end(); it_tok_end++)
//        if (it_tok_end->is_expired())
//            break;
//    if (it_tok_end != it_tok_end) {
//        m_users_mtx.read_to_write();
//        // some auth tokens expired
//        for (auto it = m_user_tokens.begin(); it != it_tok_end; it++)
//            m_local_users.erase(it->m_token);
//        m_users_mtx.write_unlock();
//    } else {
//        m_users_mtx.read_unlock();
//    }
    const auto now = std::time(nullptr);
    std::erase_if(m_local_users, [now](const auto& item) {
        const auto& [token_str, tok] = item;
        return tok.is_expired(now);
    });
    std::erase_if(m_peers, [](const auto& item) {
        const auto& [domain, peer] = item;
        return peer->m_auth.is_expired();
    });
}