#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <iostream>
#include <memory>
#include <vector>
#include <csignal>

#include "client.hpp"
#include "config.hpp"

// Global client manager for cleanup
std::unique_ptr<ClientManager> g_clientManager;

void cleanup() {
    g_clientManager.reset();
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Setup cleanup handlers
    std::atexit(cleanup);
    std::signal(SIGINT, [](int) { cleanup(); std::exit(0); });
    std::signal(SIGTERM, [](int) { cleanup(); std::exit(0); });

    // Initialize configuration
    Config::getInstance().load();

    // Create client manager
    g_clientManager = std::make_unique<ClientManager>();

    // Create first client/window
    g_clientManager->createClient();

    // Main GTK loop
    gtk_main();

    return 0;
}
