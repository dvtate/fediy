#pragma once


#include "seastar/core/future.hh"
#include "seastar/core/app-template.hh"

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
    seastar::app_template m_app;

    static seastar::app_template::config get_cfg() {
        seastar::app_template::config ret;
        ret.name = "Fed It Yourself";
        ret.description = "Self-hostable federation protocol, daemon, and app manager.";
        return ret;
    }

public:
    App(): m_app(get_cfg()) {}

    bool init() {
        if (!m_config.parse(FEDIY_CONFIG_FILE_PATH))
            return false;
        if (!m_db.connect())
            return false;
        return true;
    }

    std::shared_ptr<Peer> add_peer(std::string domain);

    void run() {

    }

    friend class Cache;
    friend class Auth;
    friend class Server;
    friend class DB;

};

// Global Singleton set in main.c
extern App* g_app;