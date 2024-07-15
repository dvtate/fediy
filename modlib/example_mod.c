#include <string.h>
#include <stdlib.h>

#include "fediymod.h"

//////////////////////////
// Exports
////////////////////////

static void handle_request(const struct fiy_request_t* request, fiy_callback_t callback) {
    // Allocate a body string to send to the user
    size_t body_len = 50
        + strlen(request->user)
        + strlen(request->domain)
        + strlen(request->path);
    char* body = (char*) malloc(sizeof(char) * body_len);
    snprintf(
        body,
        body_len,
        "Hello, @%s@%s! <br/>Path: %s",
        request->user,
        request->domain,
        request->path
    );

    struct fiy_response_t resp = {
            .status=200,
            .headers=NULL,
            .body=body
    };
    callback(request, &resp);

    // Cleanup
    free(body);
}

static void update_peer_domain(const char* old_domain, const char* new_domain) {

}

static void update_username(const char* domain, const char* old_username, const char* new_username) {}

/**
 * Initialize the module so that it can begin accepting requests
 * @param domain
 * @param module_dir
 * @return
 */
struct fiy_mod_info_t* start(const struct fiy_host_info_t* host_info) {
    // Prepare and make sure everything is set up and installed correctly
    static struct fiy_mod_info_t mod_info = {
            .on_request=handle_request,
            .on_peer_domain_changed=update_peer_domain,
            .on_username_changed=update_username
    };
    return &mod_info;
}

// Called once before sever shuts down
[[deprecated("may not get called in all cases, may terminate before call finishes")]]
void stop() { }


//