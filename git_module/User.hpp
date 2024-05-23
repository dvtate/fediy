#pragma once

#include <string>
#include <deque>
#include <memory>
#include <mutex>

#include "Repo.hpp"
#include "Contrib.hpp"

class User {
protected:
    // cache
    std::mutex m_cache_lock;
    std::deque<std::shared_ptr<Repo>> m_repos_cache;
    std::deque<std::shared_ptr<Contrib>> m_contribs_cache;

public:
    std::string m_domain;
    std::string m_name;


};