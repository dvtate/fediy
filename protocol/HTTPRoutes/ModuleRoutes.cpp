//
// Created by tate on 6/25/24.
//

#include "ModuleRoutes.hpp"

#include "LocalUser.hpp"
#include "App.hpp"

ModuleRoutes::User ModuleRoutes::find_user(const drogon::HttpRequestPtr& req) {
    // Local User authentication
    auto u = req->session()
        ->getOptional<std::shared_ptr<LocalUser>>("user").value_or(nullptr);
    if (u != nullptr)
        return { .domain=nullptr, .user=u->get_username() };

    // Peer authentication
    // TODO

    // No authentication
    return { .domain=nullptr, .user="" };
}

void ModuleRoutes::app_redirect(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    const auto m = g_app->m_mods.get_mod_by_id(req->getRoutingParameters()[0]);
    if (m == nullptr) {
        static auto ret404 = drogon::HttpResponse::newNotFoundResponse();
        callback(ret404);
    } else {
//        std::cout <<"redirecting to /" <<m->m_path <<std::endl;
        callback(drogon::HttpResponse::newRedirectionResponse("/" + m->m_path));
    }
}

void ModuleRoutes::app_send_msg(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
) {
    // Check to see if it's using a subdomain
    std::string app, uri;
    auto hostname = req->getHeader("Host");
    if (!hostname.empty()) {
        auto& hhn = g_app->m_config.m_hostname;
        if (hostname.ends_with(hhn) && hhn.size() != hostname.size()) {
            // Subdomain app  app.example.com/uri/path
            app = hostname.substr(0, hostname.size() - hhn.size() - 1);
            uri = req->path();
        } else if (hostname != hhn) {
            // Invalid request to different host?
            auto r = drogon::HttpResponse::newHttpResponse(
                drogon::HttpStatusCode::k400BadRequest,
                drogon::ContentType::CT_TEXT_HTML
            );
            r->setBody("Wrong host? Expected host to be " + hhn + " but host was " + hostname);
            callback(r);
            return;
        } else {
            // Not a subdomain app  example.com/app/uri/path
            const auto slash_idx = req->path().find('/', 1);
            std::cout <<"slash_idx = " <<slash_idx <<std::endl;
            if (slash_idx == -1) {
                app = req->path().data() + 1;
                uri = "/";
            } else {
                app = req->path().substr(1, slash_idx - 1);
                uri = req->path().data() + slash_idx;
            }
        }
    }

    // Forward to app
    DEBUG_LOG("Calling " <<app <<" : " << uri);
    Mod* m = g_app->m_mods.get_mod_by_path(app);
    if (m == nullptr) {
        callback(drogon::HttpResponse::newNotFoundResponse(req));
        return;
    }
    m->m_ipc->handle_request(req, find_user(req), std::move(callback));
}

void ModSdCheckMiddleware::invoke(
    const drogon::HttpRequestPtr& req,
    drogon::MiddlewareNextCallback&& nextCb,
    drogon::MiddlewareCallback&& mcb
) {
    // It's an app!
    static const auto& hn = g_app->m_config.m_hostname;
    if (req->getHeader("Host") != hn) {
        ModuleRoutes::app_send_msg(req, std::move(mcb));
        return;
    }

    // Go next
    nextCb(std::move(mcb));
}