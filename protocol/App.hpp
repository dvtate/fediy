#pragma once

#include "globals.hpp"

#include "Peers.hpp"
#include "DB.hpp"
#include "Config.hpp"
#include "ModMgr.hpp"
#include "Pages.hpp"

class App {

public:
    AppConfig m_config;
    Peers m_cache;
    std::unique_ptr<DB> m_db;
    ModMgr m_mods;
    std::unique_ptr<Pages> m_pages;

    App() = default;

    void run(int argc, char** argv);

    bool start();


};

// Global Singleton set in main.c
extern App* g_app;