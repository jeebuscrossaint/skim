#include "webview.hpp"
#include "config.hpp"
#include <iostream>

WebView::WebView() {
    // Create WebKit settings
    WebKitSettings* settings = webkit_settings_new();

    // Configure settings from config
    Config& config = Config::getInstance();

    webkit_settings_set_enable_javascript(settings, config.getBool("enableJavaScript", true));
    webkit_settings_set_enable_developer_extras(settings, config.getBool("enableDeveloperExtras", true));

    // Create the WebView with settings
    webView_ = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(settings));

    // Load homepage
    std::string homepage = config.getString("homepage", "https://duckduckgo.com");
    loadUri(homepage);
}
