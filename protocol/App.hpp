#pragma once


#include "seastar/core/future.hh"
#include "seastar/core/app-template.hh"

#include "globals.hpp"

#include "Cache.hpp"
#include "Auth.hpp"
#include "Server.hpp"
#include "DB.hpp"
#include "Config.hpp"
#include "ModMgr.hpp"

class App {
    static seastar::app_template::config get_cfg() {
        seastar::app_template::config ret;
        ret.name = "Fed It Yourself";
        ret.description = "Self-hostable federation protocol, daemon, and app manager.";
        return ret;
    }

public:
    AppConfig m_config;
    Cache m_cache;
    Auth m_auth;
    Server m_server;
    DB m_db;
    ModMgr m_mods;
    seastar::app_template m_app;

    App(): m_app(get_cfg()) {}


    std::shared_ptr<Peer> add_peer(std::string domain);

    void run(int argc, char** argv);
};

// Global Singleton set in main.c
extern App* g_app;