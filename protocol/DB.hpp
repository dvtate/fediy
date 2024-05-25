#pragma once

// #include <SQLiteCpp/SQLiteCpp.h>

class DB {
public:
    std::string get_user_password(std::string password);
};


// TODO db cron that prunes users+servers with expired authentication
