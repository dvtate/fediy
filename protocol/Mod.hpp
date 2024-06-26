#pragma once

#include <string>

// TODO abstract base with child classes for each method of sending requests to the modules

class Mod {
protected:
    void* m_handle{nullptr};

public:
    std::string m_id;
    std::string m_name;
    std::string m_version;
    std::string m_description;

    std::string m_error;


    explicit Mod(std::string id): m_id(std::move(id)) {}
    virtual ~Mod() = default;

    // Load module settings in metadata from fs
    bool load_conf();

//    virtual bool start() = 0;
//    virtual bool stop() = 0;
    bool start();
    bool stop();

//    static std::unique_ptr<Mod> from_id(std::string id) {
//        return nullptr;
//    }
};

//class SharedObjectMod : public Mod {
//protected:
//    void* m_handle{nullptr};
//
//public:
//    virtual bool start() override;
//    virtual bool stop() override;
//};
//
