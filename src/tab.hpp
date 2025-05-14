#pragma once

#include <memory>
#include <string>
#include <vector>
#include <gtk/gtk.h>
#include "webview.hpp"

class Tab {
public:
    Tab(GtkStack* stack);
    ~Tab();

    WebView* getWebView() { return webView_.get(); }
    std::string getId() const { return id_; }

    void focus();

private:
    std::unique_ptr<WebView> webView_;
    GtkWidget* container_;
    GtkStack* stack_;
    std::string id_;

    static std::string generateId();
};

class TabManager {
public:
    TabManager(GtkStack* stack);
    ~TabManager();

    Tab* createTab();
    void closeTab(const std::string& id);
    void nextTab();
    void prevTab();
    Tab* getCurrentTab();

private:
    GtkStack* stack_;
    std::vector<std::unique_ptr<Tab>> tabs_;
};
