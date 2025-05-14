#pragma once

#include <memory>
#include <string>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

class WebView;

class Window {
public:
    Window();
    ~Window();

    // Disable copy
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Window control functions
    void toggleFullscreen();
    void toggleMaximize();
    void show();

    // Accessors
    GtkWindow* getGtkWindow() const { return window_; }
    GtkStack* getStack() const { return stack_; }
    int getId() const { return id_; }

private:
    GtkWindow* window_;
    GtkStack* stack_;
    int id_;

    static int generateId();
};
