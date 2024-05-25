#pragma once

#include <iostream>


// TODO better logging system
#ifdef DEBUG
#define LOG_ERR(MSG) std::cerr << MSG <<std::endl;
#define LOG(MSG) std::cout << MSG <<std::endl;
#define DEBUG_LOG(MSG) std::cout << MSG <<std::endl;
#else
// maybe write these to a file?
#define LOG_ERR(MSG) std::cerr << MSG <<std::endl;
#define LOG(MSG) std::cout << MSG <<std::endl;
#define DEBUG_LOG(MSG) ;
#endif
