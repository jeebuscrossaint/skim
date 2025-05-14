#pragma once

#include <memory>
#include <string>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

class WebView {
public:
    WebView();
    ~WebView();

    // Navigation functions
    void loadUri(const std::string& uri);
    void goBack();
    void goForward();
    void reload();
    void stopLoading();

    // Zoom functions
    void zoomIn();
    void zoomOut();
    void resetZoom();

    // Scroll functions
    void scrollDown();
    void scrollUp();
    void scrollToTop();
    void scrollToBottom();

    // Accessors
    WebKitWebView* getNativeWebView() const { return webView_; }

private:
    WebKitWebView* webView_;
    void runJavaScript(const std::string& script);
};
