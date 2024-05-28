#pragma once

#include <string>


class Mod {
public:
    std::string m_id;
    std::string m_name;
    std::string m_version;
    std::string m_description;

    std::string m_error;

    explicit Mod(std::string id): m_id(std::move(id)) {}

    // Load module settings in metadata from fs
    bool load_conf();

    //
    bool start_module();

};

