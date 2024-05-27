#pragma once

#include <string>
#include <memory>
#include <ctime>
#include <cstring>

class PeerAuth {
    /// Bearer token for authenticating endpoints
    std::string m_bearer_token_they_use;
    std::string m_bearer_token_we_use;

    /// AES token for sending encrypted data
    std::string m_aes_key; // TODO protect this from other threads, securely delete, etc.

    /// GPG public key
    std::string m_pubkey;

    /// When do we need to refresh auth credentials?
    time_t m_expire_ts;

public:
    PeerAuth(std::string token, std::string sym_key, std::string pubkey, const time_t expire_ts):
        m_bearer_token(std::move(token)),
        m_aes_key(std::move(sym_key)),
        m_pubkey(std::move(pubkey)),
        m_expire_ts(expire_ts)
    {}

    [[nodiscard]] bool is_expired(const time_t now = std::time(nullptr)) const {
        return now > m_expire_ts;
    }

    bool refresh();

    std::string aes_encrypt(const std::string& msg);
    std::string hash_with_bearer(std::string msg);

};

// This is another server on a different domain
class Peer {
public:
    PeerAuth m_auth;
    char* m_domain;

    Peer(const std::string& domain, PeerAuth auth):
        m_auth(std::move(auth))
    {
        m_domain = (char*) malloc(domain.size() + 1);
        strcpy(m_domain, domain.c_str());
    }
    Peer(char* domain, PeerAuth auth):
        m_auth(auth), m_domain(domain) 
    {}

    ~Peer() {
        free(m_domain);
    }

};