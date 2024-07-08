//
// Created by tate on 7/5/24.
//

#include "fediymod.h"


//////////////////////////
// Exports
////////////////////////

static void handle_request(fediy::fiy_request_t* request, fediy::fiy_callback_t callback) {
    std::string body = "Hello, @" + std::string(request->user) + "@" + request->domain
          + "! <br/>Path: " + request->path;

    fediy::fiy_response_t r={
            .status=200,
            .body=body.c_str()
    };
    callback(&r);
}

