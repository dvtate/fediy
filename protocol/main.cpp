
#include "drogon/drogon.h"

#include "App.hpp"

// Global App singleton
App* g_app;

int main(int argc, char** argv) {
    // Start app services
    g_app = new App();
    g_app->start();



    using namespace std::chrono_literals;
    // All sessions are stored for 24 Hours
    drogon::app()
        .enableSession(0)
        .addListener("127.0.0.1", 8848)
        .run();
}