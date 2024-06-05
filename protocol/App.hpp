#pragma once

#include "seastar/core/future.hh"
#include "seastar/core/app-template.hh"

#include "globals.hpp"

#include "Cache.hpp"
#include "DB.hpp"
#include "Config.hpp"
#include "ModMgr.hpp"
#include "Pages.hpp"

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
    std::unique_ptr<DB> m_db;
    ModMgr m_mods;
    seastar::app_template m_app;
    std::unique_ptr<Pages> m_pages;

    App(): m_app(get_cfg()) {}

    void run(int argc, char** argv);

    void first_start() {
        // Create admin user

    }

};

// Global Singleton set in main.c
extern App* g_app;