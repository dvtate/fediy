#pragma once

#include "Peer.hpp"
#include "LocalUser.hpp"

namespace ProtocolActions {
    // Functions without a domain parameter are outgoing local handlers

    // These get sent out to all our apps as well as all our linked peers
    namespace send {
        // Local user changed their username
//        seastar::future<> username_changed(const std::string& old_username, const std::string& new_username);
    }

    namespace recv {
        // Peer's user changed their username
//        seastar::future<> username_changed(const std::string& domain, const std::string& old_username, const std::string& new_username);

        // Establish relationship with a previously unknown peer
        //  OR refresh peer's authentication
//        seastar::future<> new_peer(const std::string& domain);

        // Peer moved to a new domain
//        seastar::future<> peer_domain_changed(const std::string& old_domain, const std::string& new_domain);


    }

    namespace local_recv {
        //

        // User needs a refresh token
//        seastar::future<> user_login(const std::string& username, const std::string& password);
    }
};
