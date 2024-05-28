//
// Created by tate on 5/25/24.
//
#include "DB.hpp"

#include "App.hpp"


bool DB::connect() {
    m_db = std::make_unique<SQLite::Database>(g_app->m_config.m_data_dir + "/db.db3", SQLite::OPEN_FULLMUTEX | SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE );
}
