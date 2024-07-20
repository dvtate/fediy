//
// Created by tate on 6/25/24.
//

#include "drogon/HttpClient.h"

#include "App.hpp"

#include "PeerRoutes.hpp"

void PeerRoutes::pubkey(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    static const auto ret = drogon::HttpResponse::newFileResponse(
        g_app->m_config.m_data_dir + "/auth/pubkey"
    );
    callback(ret);
}

void PeerRoutes::handshake(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    // Use private key associated with our pubkey endpoint to decrypt request body

    // Store provided peer data: host, symkey, token, etc. into Peers cache

    // Get relevant public key
//    auto client = drogon::HttpClient::newHttpClient(remote_peer);

    // Send token + symkey, encrypted with the user's pubkey
}