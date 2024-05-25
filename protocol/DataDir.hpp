#pragma once
#include "Config.hpp"

/*
    data_dir/
        db.sql      - database
        pubkey      -
        config.ini  - global admin settings

        apps/
            example.app.id/
                config.json
                index.so

*/

class DataDir {
    std::string m_path;
public:
    DataDir(std::string path): m_path(path) {}

    bool is_setup();
    bool setup();

};