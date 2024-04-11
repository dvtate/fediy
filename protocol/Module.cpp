#include <fstream>
#include <exception>
#include <cstdio>
#include <cstring>

#include "../third_party/json/single_include/nlohmann/json.hpp"

#include "Module.hpp"

Module::Module(const std::string& path) {
    // Iterate over keys
    auto conf = nlohmann::json::parse(std::ifstream(path));
    if (!conf.is_object())
        throw std::runtime_error("invalid module.json: should be an object");
    conf = conf.object();
    for (auto& [key, value] : conf.items()) {
        if (key == "id") {
            if (!value.is_string())
                throw std::runtime_error("invalid module.json: id should be a string");

            auto id = value.template get<std::string>();
            char* s = (char*) malloc(id.size() + 1);
            strcpy(s, id.c_str());
            m_info.m_name = s;
        }
    }
}

Module::~Module() {
    if (m_info.m_name != nullptr)
        free((void*) m_info.m_name);
}

void Module::start() {

}


void Module::stop() {

}


bool Module::send_message(Msg& msg) {

}
