#pragma once

#include "../util/defs.hpp"

#include "Cache.hpp"
#include "Auth.hpp"
#include "Server.hpp"
#include "DB.hpp"

class App {
protected:
    Cache m_cache;
    Auth m_auth;
    Server m_server;
    DB m_db;
    
public:
    App() {}

    std::shared_ptr<Peer> add_peer(std::string domain);

    friend class Cache;
    friend class Auth;
    friend class Server;
    friend class DB;

};

// Glboal Singleton set in main.c
extern App* g_app;