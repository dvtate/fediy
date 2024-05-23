#include "Cache.hpp"


std::shared_ptr<Peer> Cache::get_peer(const std::string& domain) {        
    m_peers_mtx.read_lock();

    if (m_peers.contains(domain)) {
        auto ret = m_peers[domain];
        m_peers_mtx.read_unlock();
        return ret;
    } else {
        m_peers_mtx.read_unlock();
        return g_app->add_peer(domain);
    }
}

std::shared_ptr<Peer> Cache::get_user(const std::string& username) {
    
}