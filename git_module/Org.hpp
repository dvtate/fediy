#pragma once


#include <memory>
#include <mutex>
#include <deque>
#include <string>

#include "Repo.hpp"
#include "User.hpp"

class Org {
protected:
    // cache
    std::mutex m_cache_lock;
    std::deque<std::shared_ptr<Repo>> m_repos_cache;
    std::deque<std::shared_ptr<User>> m_members_cache;

public:
    std::string m_domain;
    std::string m_name;
};