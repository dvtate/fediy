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
    return g_app->add_peer(domain);
}

/**
 * \returns null when invalid user token
 */
std::shared_ptr<LocalUser> Cache::get_user(const std::string& token) {
    m_users_mtx.read_lock();

    auto it = m_local_users.find(token);
    if (it != m_local_users.end()) {
        m_users_mtx.read_unlock();
        return it->second;
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