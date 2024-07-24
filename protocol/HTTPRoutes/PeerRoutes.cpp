//
// Created by tate on 6/25/24.
//

#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

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
//
//std::string read_pubkey() {
//
//}
//
//std::string decrypt_message(std::string& msg) {
//
//}
//
//// same content both ways
//struct HandshakeMsg {
//    bool sig_valid;
//    std::string domain;
//    std::string token;
//    std::string symkey;
//
//    std::string to_string() {
//        return this->domain + "\n" + this->token + "\n" + this->symkey;
//    }
//    static HandshakeMsg from_string(std::string&& str) {
//        HandshakeMsg ret;
//
//    }
//};
//
//
//
//HandshakeMsg parse_handshake(std::string body) {
//    body = drogon::utils::base64Decode(body);
//
//}
//std::string write_handshake(HandshakeMsg msg);

void PeerRoutes::handshake(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    // Use private key associated with our pubkey endpoint to decrypt request body

    // Store provided peer data: host, symkey, token, etc. into Peers cache

//    auto body = drogon::utils::base64Decode(req->body()); // may contain \0

    // Get relevant public key
//    auto client = drogon::HttpClient::newHttpClient(remote_peer);
//    client.request()

    // Send token + symkey, encrypted with the user's pubkey
}