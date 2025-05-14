#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <iostream>
#include <memory>
#include <vector>
#include <csignal>

#include "client.hpp"
#include "config.hpp"
#include "keymap.hpp"
#include "tab.hpp"

// Global client manager for cleanup
std::unique_ptr<ClientManager> g_clientManager;

void setupKeybindings(Window* window, TabManager* tabManager) {
    KeyMap& keymap = KeyMap::getInstance();

    // Navigation keys
    keymap.registerKey("ctrl+l", [tabManager]() {
        // Focus URL bar (would require adding URL bar to the UI)
        std::cout << "URL bar focus requested" << std::endl;
        return true;
    });

    keymap.registerKey("ctrl+r", [tabManager]() {
        if (Tab* tab = tabManager->getCurrentTab()) {
            tab->getWebView()->reload();
        }
        return true;
    });

    keymap.registerKey("alt+Left", [tabManager]() {
        if (Tab* tab = tabManager->getCurrentTab()) {
            tab->getWebView()->goBack();
        }
        return true;
    });

    keymap.registerKey("alt+Right", [tabManager]() {
        if (Tab* tab = tabManager->getCurrentTab()) {
            tab->getWebView()->goForward();
        }
        return true;
    });

    // Tab management keys
    keymap.registerKey("ctrl+t", [tabManager]() {
        tabManager->createTab();
        return true;
    });

    keymap.registerKey("ctrl+w", [tabManager]() {
        if (Tab* tab = tabManager->getCurrentTab()) {
            tabManager->closeTab(tab->getId());
        }
        return true;
    });

    keymap.registerKey("ctrl+Tab", [tabManager]() {
        tabManager->nextTab();
        return true;
    });

    keymap.registerKey("ctrl+shift+Tab", [tabManager]() {
        tabManager->prevTab();
        return true;
    });

    // Window control keys
    keymap.registerKey("F11", [window]() {
        window->toggleFullscreen();
        return true;
    });

    keymap.registerKey("ctrl+q", []() {
        gtk_main_quit();
        return true;
    });
}

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

    // Create client manager and first client
    g_clientManager = std::make_unique<ClientManager>();
    auto client = g_clientManager->createClient();

    // Create tab manager
    auto tabManager = std::make_unique<TabManager>(client->getWindow().getStack());

    // Setup key bindings
    setupKeybindings(&client->getWindow(), tabManager.get());

    // Connect key-press-event to window
    g_signal_connect(client->getWindow().getGtkWindow(), "key-press-event",
                    G_CALLBACK(+[](GtkWidget* widget, GdkEventKey* event, gpointer data) {
                        return KeyMap::getInstance().processKeyEvent(event) ? TRUE : FALSE;
                    }), nullptr);

    // Main GTK loop
    gtk_main();

    return 0;
}
