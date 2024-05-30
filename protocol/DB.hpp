#pragma once

#include <mutex>

// #include <SQLiteCpp/SQLiteCpp.h>
#include "../third_party/SQLiteCpp/include/SQLiteCpp/SQLiteCpp.h"

#include "Config.hpp"
#include "Peer.hpp"
#include "LocalUser.hpp"

class DB {
public:
    // TODO use connection pool for requests
    //  https://www.sqlite.org/threadsafe.html
    SQLite::Database m_db;

private:
    // Precompiled queries
    SQLite::Statement m_get_user_query;
    SQLite::Statement m_add_user_query;
    SQLite::Statement m_get_peer_query;
    SQLite::Statement m_add_peer_query;

protected:
    std::mutex m_mtx;

public:

    explicit DB(const std::string& db_file);

    std::shared_ptr<Peer> get_peer(std::string domain);

    std::shared_ptr<LocalUser> get_user(const std::string& username);
    std::shared_ptr<LocalUser> get_user(const std::string& username, std::string password);
//    std::shared_ptr<LocalUser> add_user();

};


// TODO db cron that prunes users+servers with expired authentication
