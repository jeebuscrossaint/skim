#include "window.hpp"
#include "webview.hpp"
#include "config.hpp"
#include <iostream>
#include <random>

int Window::generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 1000000);
    return dis(gen);
}

Window::Window() : id_(generateId()) {
    // Create GTK window
    window_ = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

    // Set window properties
    gtk_window_set_title(window_, "Skim Browser");

    // Get window size from config
    Config& config = Config::getInstance();
    int width = config.getInt("width", 1024);
    int height = config.getInt("height", 768);

    gtk_window_set_default_size(window_, width, height);

    // Create stack for tabs
    stack_ = GTK_STACK(gtk_stack_new());

    // Create initial WebView
    auto webView = new WebView();

    // Create a box to hold the WebView
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(webView->getNativeWebView()), TRUE, TRUE, 0);

    // Add the box to the stack
    gtk_stack_add_named(stack_, box, "tab1");

    // Add the stack to the window
    gtk_container_add(GTK_CONTAINER(window_), GTK_WIDGET(stack_));

    // Connect destroy signal
    g_signal_connect(window_, "destroy", G_CALLBACK(+[](GtkWidget* widget, gpointer data) {
        gtk_main_quit();
    }), nullptr);

    // Connect key press event
    g_signal_connect(window_, "key-press-event", G_CALLBACK(+[](GtkWidget* widget, GdkEventKey* event, gpointer data) {
        Window* self = static_cast<Window*>(data);

        // Handle Ctrl+Q to quit
        if (event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_q) {
            gtk_main_quit();
            return TRUE;
        }

        // Handle Ctrl+T for new tab
        if (event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_t) {
            // TODO: Implement new tab functionality
            return TRUE;
        }

        // Handle Ctrl+W to close tab
        if (event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_w) {
            // TODO: Implement close tab functionality
            return TRUE;
        }

        // Handle Ctrl+L to focus address bar
        if (event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_l) {
            // TODO: Implement focus address bar functionality
            return TRUE;
        }

        // Handle Ctrl+R to reload
        if (event->state & GDK_CONTROL_MASK && event->keyval == GDK_KEY_r) {
            // TODO: Reload the current page
            return TRUE;
        }

        return FALSE;
    }), this);


    // Show all
    gtk_widget_show_all(GTK_WIDGET(window_));
}

Window::~Window() {
    // GTK will handle destroying the window_
}

void Window::toggleFullscreen() {
    static bool isFullscreen = false;

    if (isFullscreen) {
        gtk_window_unfullscreen(window_);
    } else {
        gtk_window_fullscreen(window_);
    }

    isFullscreen = !isFullscreen;
}

void Window::toggleMaximize() {
    if (gtk_window_is_maximized(window_)) {
        gtk_window_unmaximize(window_);
    } else {
        gtk_window_maximize(window_);
    }
}

void Window::show() {
    gtk_widget_show_all(GTK_WIDGET(window_));
}
