#include "webview.hpp"
#include "config.hpp"
#include <iostream>

WebView::WebView() {
    // Create WebKit settings
    WebKitSettings* settings = webkit_settings_new();

    // Configure settings from config or use defaults
    webkit_settings_set_enable_javascript(settings, TRUE);
    webkit_settings_set_enable_smooth_scrolling(settings, TRUE);
    webkit_settings_set_enable_developer_extras(settings, TRUE);
    webkit_settings_set_enable_webgl(settings, TRUE);

    // Create the WebView with settings
    webView_ = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(settings));

    // Load homepage
    std::string homepage = Config::getInstance().getString("homepage", "https://duckduckgo.com");
    loadUri(homepage);
}

WebView::~WebView() {
    // WebKit handles cleanup of the webView_ internally through GObject
}

void WebView::loadUri(const std::string& uri) {
    std::string processedUri = uri;

    // Check if URI has a scheme
    if (uri.find("://") == std::string::npos) {
        // If it looks like a URL, add https
        if (uri.find(".") != std::string::npos) {
            processedUri = "https://" + uri;
        } else {
            // Treat as a search term
            std::string searchEngine = Config::getInstance().getString(
                "searchEngine", "https://duckduckgo.com/?q=%s");
            size_t pos = searchEngine.find("%s");
            if (pos != std::string::npos) {
                searchEngine.replace(pos, 2, uri);
                processedUri = searchEngine;
            }
        }
    }

    webkit_web_view_load_uri(webView_, processedUri.c_str());
}

void WebView::goBack() {
    if (webkit_web_view_can_go_back(webView_)) {
        webkit_web_view_go_back(webView_);
    }
}

void WebView::goForward() {
    if (webkit_web_view_can_go_forward(webView_)) {
        webkit_web_view_go_forward(webView_);
    }
}

void WebView::reload() {
    webkit_web_view_reload(webView_);
}

void WebView::runJavaScript(const std::string& script) {
    webkit_web_view_run_javascript(webView_, script.c_str(), nullptr, nullptr, nullptr);
}

void WebView::scrollDown() {
    runJavaScript("window.scrollBy(0, 100);");
}

void WebView::scrollUp() {
    runJavaScript("window.scrollBy(0, -100);");
}
