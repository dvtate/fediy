
///////
// This should go in shared header
///////

struct fiy_request_t {
    const char* url;
    const char* body;         // null = get request
    const char* domain;       // null = local
    const char* user;         // null = unauthenticated
    const char* headers;      // is this even needed?
};

struct fiy_response_t {
    char* headers;          // updated headers
    int status;             //
    char* body;             //
};

//
struct fiy_host_info_t {
    const char* domain;
};

/// This is used to provide callbacks to the host app
struct fiy_mod_info_t {
    /// username changed
    // if domain is null, then user is local
    void (*username_change_handler)(const char* domain, const char* old_username, const char* new_username);

    /// peer domain changed
    void (*peer_domain_change_handler)(const char* old_domain, const char* new_domain);

    /// Handle http requests to the module
    // TODO this typedef will probably change
    void (*request_handler)(const struct fiy_request_t* request);
};

#ifdef __cplusplus
namespace fediy {
    typedef struct fiy_request_t Request;
    typedef struct fiy_host_info_t HostInfo;
    typedef struct fiy_mod_info_t ModInfo;
};
#endif

//////////////////////////
// Exports
////////////////////////

void f(int a, const char* c);

static void handle_request(const struct fiy_request_t* request) {
    /// business logic and frontend hosting
}

static void update_peer_domain(const char* old_domain, const char* new_domain) {

}
static void update_username(const char* domain, const char* old_username, const char* new_username) {}
static struct fiy_mod_info_t mod_info = {
    .request_handler=handle_request,
    .peer_domain_change_handler=update_peer_domain,
    .username_change_handler=update_username
};
struct fiy_mod_info_t* start(const struct fiy_host_info_t* host_info) {
    // prepare and make sure everything is set up and installed correctly
    return &mod_info;
}

// Called once before sever shuts down
[[deprecated("may not get called in all cases, may terminate before call finishes")]]
void stop() { }


//