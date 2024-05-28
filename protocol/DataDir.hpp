#pragma once
#include "Config.hpp"

/*

*/

class DataDir {
    std::string m_path;
public:
    DataDir(std::string path): m_path(path) {}

    bool is_setup();
    bool setup();

};