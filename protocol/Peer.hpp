#pragma once

#include <string>
#include <memory>
#include <ctime>
#include <cstring>
#include <random>

class PeerAuth {
public:
    /// AES token for sending encrypted data
    std::string m_aes_key; // TODO mem protect, securely delete, etc.

    /// GPG public key
    std::string m_pubkey;

    /// Bearer token for authenticating endpoints
    std::string m_bearer_token_we_send;
    std::string m_bearer_token_we_accept;

    /// When do we need to refresh auth credentials?
    time_t m_expire_ts;

    static constexpr time_t SESSION_LIFETIME = 60 * 60 * 24 * 7; // 1 week
    static constexpr int TOKEN_LEN = 24;

    static std::string get_token_string() {
        // Create random generator that picks indices charset
        // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
        static thread_local std::random_device rd;
        static thread_local std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned short> dist(1, 63);
        const char charset[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

        // Generate token using random chars from charset
        // Probably a better way to do this that doesn't use operator+=
        std::string ret;
        ret.reserve(TOKEN_LEN);
        ret += '.'; // start with '.' to differentiate from user tokens
        for (int i = 1; i < TOKEN_LEN; i++)
            ret += charset[dist(gen)];
        return ret;
    }

public:

    PeerAuth(
        std::string sym_key,
        std::string pubkey,
        std::string peer_provided_token,
        std::string our_generated_token = get_token_string(),
        const time_t expire_ts = std::time(nullptr) + SESSION_LIFETIME
    ):
        m_aes_key(std::move(sym_key)),
        m_pubkey(std::move(pubkey)),
        m_bearer_token_we_send(std::move(peer_provided_token)),
        m_bearer_token_we_accept(std::move(our_generated_token)),
        m_expire_ts(expire_ts)
    {}

    [[nodiscard]] bool is_expired(const time_t now = std::time(nullptr)) const {
        return now > m_expire_ts;
    }

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