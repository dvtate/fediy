
#include <mutex>
#include <string>
#include <memory>
#include <vector>

#include "GitRepo.hpp"

class Repo {
    // Which server is hosting the repo
    std::string m_domain;

    // Which user owns the repo
    std::string m_username;

    // What is the name of the repo
    std::string m_reponame;

};

// How are we gonna show users?? emails no longer necessarily globally unique...
