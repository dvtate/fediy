//
// Created by tate on 7/5/24.
//

#ifndef FEDIY_FEDIYMOD_H
#define FEDIY_FEDIYMOD_H

#ifndef __cplusplus
#include <stdio.h>
#else
#include <string>
#endif

#ifdef __cplusplus
namespace fediy {
#endif

struct fiy_request_t {
    const char* path;
    const char* body;         // null = get request
    const char* domain;       // null = local
    const char* user;         // null = unauthenticated
    const char* headers;      // is this even needed?
};

struct fiy_response_t {
#ifndef __cplusplus
    int status;                     // exit status
    const char* body;               // body
    const char* headers;            // updated headers or null
#else
    int status{200};                // exit status
    const char* body{nullptr};      // body
    const char* headers{nullptr};   // updated headers or null
#endif
};

typedef void (* fiy_callback_t)(const struct fiy_request_t* request, const struct fiy_response_t*);

/// This is used to provide callbacks to the host app
struct fiy_mod_info_t {
    /// Handle http requests to the module
    void (*on_request)(const struct fiy_request_t* request, fiy_callback_t callback);

    /// peer domain changed
    void (*on_peer_domain_changed)(const char* old_domain, const char* new_domain);

    /// username changed
    // if domain is null, then user is local
    void (*on_username_changed)(const char* domain, const char* old_username, const char* new_username);
};

struct fiy_host_info_t {
    const char* base_uri; // <protocol>://<host>/<appid> ie - https://bodge.dev/git
    void (*log)(int level, const char* message);
};

typedef struct fiy_mod_info_t* (*fiy_mod_start_function_t)(const struct fiy_host_info_t*);

#ifdef __cplusplus
    struct ModInfo : public fiy_mod_info_t {};
//    struct Request : public fiy_request_t {};
//    struct HostInfo : public fiy_host_info_t {};
//    struct Response : public fiy_response_t {
//        std::string body;
//        std::string headers;
//        int status;
//        explicit Response(std::string body, int status = 200):
//            body(std::move(body)),
//            status(status)
//        {}
//
//        ~Response() {}
//
//        operator struct fiy_response_t() {
//            return (struct fiy_response_t){
//                .status=status,
//                .body=body.c_str(),
//                .headers=headers.c_str()
//            };
//        }
//    };
#endif


#ifdef __cplusplus
} // namespace fediy
#endif

#endif //FEDIY_FEDIYMOD_H
