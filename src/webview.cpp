#include "webview.hpp"
#include "config.hpp"
#include <iostream>

WebView::WebView() {
    // Create WebKit settings
    WebKitSettings* settings = webkit_settings_new();

    // Configure settings from config
    Config& config = Config::getInstance();

    webkit_settings_set_enable_javascript(settings, config.getBool("enableJavaScript", true));
    webkit_settings_set_enable_smooth_scrolling(settings, config.getBool("enableSmoothScrolling", true));
    webkit_settings_set_enable_developer_extras(settings, config.getBool("enableDeveloperExtras", true));
    webkit_settings_set_enable_webgl(settings, config.getBool("enableWebGL", true));

    // Create the WebView with settings
    webView_ = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(settings));

    // Connect load-changed signal to update title
    g_signal_connect(webView_, "load-changed", G_CALLBACK(+[](WebKitWebView* webView, WebKitLoadEvent event, gpointer userData) {
        if (event == WEBKIT_LOAD_FINISHED) {
            // You could update window title or tab title here
            const char* title = webkit_web_view_get_title(webView);
            if (title) {
                // Update title
                GtkWidget* window = gtk_widget_get_toplevel(GTK_WIDGET(webView));
                if (GTK_IS_WINDOW(window)) {
                    gtk_window_set_title(GTK_WINDOW(window), title);
                }
            }
        }
    }), nullptr);

    // Load homepage
    std::string homepage = config.getString("homepage", "https://duckduckgo.com");
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

void WebView::stopLoading() {
    webkit_web_view_stop_loading(webView_);
}

void WebView::zoomIn() {
    double zoom = webkit_web_view_get_zoom_level(webView_);
    webkit_web_view_set_zoom_level(webView_, zoom + 0.1);
}

void WebView::zoomOut() {
    double zoom = webkit_web_view_get_zoom_level(webView_);
    webkit_web_view_set_zoom_level(webView_, zoom - 0.1);
}

void WebView::resetZoom() {
    webkit_web_view_set_zoom_level(webView_, 1.0);
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

void WebView::scrollToTop() {
    runJavaScript("window.scrollTo(0, 0);");
}

void WebView::scrollToBottom() {
    runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}
