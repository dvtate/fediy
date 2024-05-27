#pragma once

// #include <SQLiteCpp/SQLiteCpp.h>
#include "../third_party/SQLiteCpp/include/SQLiteCpp/SQLiteCpp.h"

#include "Config.hpp"
#include "Peer.hpp"

class DB {
    // TODO use connection pool for requests
    //  https://www.sqlite.org/threadsafe.html
    std::unique_ptr<SQLite::Database> m_db{nullptr};

public:

    bool connect();

    std::string get_user_password(std::string password);

    std::shared_ptr<Peer> get_peer(std::string domain);

    bool setup_db();
};


// TODO db cron that prunes users+servers with expired authentication
