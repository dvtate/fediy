#pragma once

#include "../util/defs.hpp"

#include "Cache.hpp"
#include "Auth.hpp"
#include "Server.hpp"
#include "DB.hpp"
#include "Config.hpp"

class App {
    AppConfig m_config;
    Cache m_cache;
    Auth m_auth;
    Server m_server;
    DB m_db;

public:
    App() {
        m_config.parse(FEDIY_CONFIG_FILE_PATH);
        m_db.connect();
    }

    std::shared_ptr<Peer> add_peer(std::string domain);

    friend class Cache;
    friend class Auth;
    friend class Server;
    friend class DB;

};

// Global Singleton set in main.c
extern App* g_app;