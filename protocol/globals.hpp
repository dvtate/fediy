#pragma once

#include "../util/defs.hpp"

#ifndef FEDIY_CONFIG_FILE_PATH
#define FEDIY_CONFIG_FILE_PATH "/opt/fediy/config.ini"
#endif

// Global app singleton set in main()
class App;
extern App* g_app;
