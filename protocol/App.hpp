#pragma once

#include "globals.hpp"

#include "Peers.hpp"
#include "DB.hpp"
#include "Config.hpp"
#include "Mods.hpp"
#include "Pages.hpp"

/**
 * Global protocol server app singleton
 */
class App {

public:
    AppConfig m_config;
    Peers m_peers;
    std::unique_ptr<DB> m_db;
    Mods m_mods;
    std::unique_ptr<Pages> m_pages;

    App() = default;
    explicit App(const std::string& config_path):
        m_config(config_path)
    {}

    bool start();
};

// Global Singleton set in main.c
extern App* g_app;