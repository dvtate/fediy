#include "Peers.hpp"

#include "App.hpp"


/**
 * \returns null when not a peer invalid user token
 */
std::shared_ptr<Peer> Peers::get_peer_for_domain(const std::string& domain) {
    RWMutex::LockForRead lock{m_mtx};

    auto it = m_peers_out.find(domain);
    if (it != m_peers_out.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<Peer> Peers::get_peer_from_token(const std::string& domain) {
    RWMutex::LockForRead lock{m_mtx};

    auto it = m_peers_out.find(domain);
    if (it != m_peers_out.end())
        return it->second;
    return nullptr;
}

void Peers::prune() {
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
    std::erase_if(m_peers_in, [this](const auto& item) {
        const auto& [domain, peer] = item;
        if (peer->m_auth.is_expired()) {
            m_peers_out.erase(peer->m_domain);
            return true;
        }
        return false;
    });
}