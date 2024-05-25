#include "Cache.hpp"

#include "App.hpp"


std::shared_ptr<Peer> Cache::get_peer(const std::string& domain) {
    m_peers_mtx.read_lock();

    if (m_peers.contains(domain)) {
        auto ret = m_peers[domain];
        m_peers_mtx.read_unlock();
        return ret;
    }
    m_peers_mtx.read_unlock();
    return g_app->add_peer(domain);
}

/**
 * \returns null when invalid user token
 */
std::shared_ptr<LocalUser> Cache::get_user(const std::string& token) {
    m_users_mtx.read_lock();

    if (m_local_users.contains(token)) {
        auto ret = m_local_users[token];
        m_users_mtx.read_unlock();
        return ret;
    }

    // Probably invalid token
    return nullptr;
}

void Cache::prune() {
    std::erase_if(m_local_users, [](const auto& item) {
        const auto& [token, user] = item;
        return user->is_auth_expired();
    });
    std::erase_if(m_peers, [](const auto& item) {
        const auto& [domain, peer] = item;
        return peer->m_auth.is_expired();
    });    
}