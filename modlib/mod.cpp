//
// Created by tate on 7/5/24.
//

#include "fediymod.h"


//////////////////////////
// Exports
////////////////////////

static void handle_request(const fediy::fiy_request_t* request, fediy::fiy_callback_t callback) {
    std::string body = "Hello, @" + std::string(request->user) + "@" + request->domain
          + "! <br/>Path: " + request->path;

    fediy::fiy_response_t r={
//            .status=200,
            .body=body.c_str()
    };
    callback(request, &r);
}
//
//// if domain is null, then user is local
//void (* username_change_handler)(const char* domain, const char* old_username, const char* new_username);
//
///// peer domain changed
//void (* peer_domain_change_handler)(const char* old_domain, const char* new_domain);
//


fediy::fiy_mod_info_t* start(const fediy::fiy_host_info_t* host_info) {
    static fediy::fiy_mod_info_t mod_info = {
        .on_request=handle_request,
        .on_peer_domain_changed=nullptr,
        .on_username_changed=nullptr,
    };
    return &mod_info;
}
