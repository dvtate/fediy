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
        throw std::runtime_error("Invalid module.json: should be an object");
    conf = conf.object();
    for (auto& [key, value] : conf.items()) {
        if (key == "id") {
            if (!value.is_string())
                throw std::runtime_error("Invalid module.json: id should be a string");

            auto id = value.template get<std::string>();
            char* s = (char*) malloc(id.size() + 1);
            strcpy(s, id.c_str());
            m_info.m_name = s;
        } else if (key == "version_major") {
            if (!value.is_number())
                throw std::runtime_error("Invalid module.json: version_major should be an integer");
            auto v = value.template get<uint32_t>();
            m_info.m_version_major = v;
        } else if (key == "version_minor") {
            if (!value.is_number())
                throw std::runtime_error("Invalid module.json: version_minor should be an integer");
            auto v = value.template get<uint32_t>();
            m_info.m_version_minor = v;
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
