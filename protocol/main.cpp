#include "../third_party/seastar/include/seastar/core/reactor.hh"
#include "../third_party/seastar/include/seastar/core/app-template.hh"

#include "App.hpp"

// Global App singleton
App* g_app;

int main(int argc, char** argv) {
    seastar::app_template app;
    app.run(argc, argv, [] {
        std::cout << seastar::smp::count << "\n";
        g_app = new App();
        return seastar::make_ready_future<>();
    });
}