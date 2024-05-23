#pragma once

#include <iostream>

#ifdef DEBUG
#define LOG_ERR(MSG) std::cerr << MSG <<std::endl;
#define LOG(MSG) std::cout << MSG <<std::endl;
#else
// maybe write these to a file?
#define LOG_ERR(MSG) std::cerr << MSG <<std::endl;
#define LOG(MSG) std::cout << MSG <<std::endl;
#endif

// TODO make logging system

