#include "tab.hpp"
#include <random>
#include <sstream>
#include <algorithm>

std::string Tab::generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 1000000);

    std::stringstream ss;
    ss << "tab" << dis(gen);
    return ss.str();
}

Tab::Tab(GtkStack* stack) : stack_(stack), id_(generateId()) {
    // Create WebView
    webView_ = std::make_unique<WebView>();

    // Create container for WebView
    container_ = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(container_), GTK_WIDGET(webView_->getNativeWebView()), TRUE, TRUE, 0);

    // Add to stack
    gtk_stack_add_named(stack_, container_, id_.c_str());
    gtk_widget_show_all(container_);
}

Tab::~Tab() {
    // Remove container from stack if it's still there
    if (container_ && GTK_IS_WIDGET(container_) && gtk_widget_get_parent(container_)) {
        gtk_container_remove(GTK_CONTAINER(stack_), container_);
    }
}

void Tab::focus() {
    gtk_stack_set_visible_child_name(stack_, id_.c_str());
}

TabManager::TabManager(GtkStack* stack) : stack_(stack) {
    // Create initial tab
    createTab();
}

TabManager::~TabManager() {
    // Tabs are automatically cleaned up by unique_ptr
}

Tab* TabManager::createTab() {
    auto tab = std::make_unique<Tab>(stack_);
    Tab* tabPtr = tab.get();

    tabs_.push_back(std::move(tab));
    tabPtr->focus();

    return tabPtr;
}

void TabManager::closeTab(const std::string& id) {
    auto it = std::find_if(tabs_.begin(), tabs_.end(),
                          [&id](const std::unique_ptr<Tab>& tab) {
                              return tab->getId() == id;
                          });

    if (it != tabs_.end()) {
        // If we're closing the current tab, switch to another one
        bool wasCurrent = (getCurrentTab() == it->get());

        // Get index of the tab to be closed
        size_t index = std::distance(tabs_.begin(), it);

        // Remove the tab
        tabs_.erase(it);

        // If we closed the current tab and there are other tabs, focus on one of them
        if (wasCurrent && !tabs_.empty()) {
            // If we closed the last tab, focus on the new last tab
            // Otherwise focus on the tab that took the closed tab's place
            size_t newIndex = (index >= tabs_.size()) ? tabs_.size() - 1 : index;
            tabs_[newIndex]->focus();
        }

        // If we closed the last tab, create a new one
        if (tabs_.empty()) {
            createTab();
        }
    }
}

void TabManager::nextTab() {
    if (tabs_.empty()) return;

    // Find current tab
    Tab* current = getCurrentTab();
    if (!current) return;

    // Find index of current tab
    auto it = std::find_if(tabs_.begin(), tabs_.end(),
                          [current](const std::unique_ptr<Tab>& tab) {
                              return tab.get() == current;
                          });

    if (it != tabs_.end()) {
        // Move to next tab or wrap around
        size_t index = std::distance(tabs_.begin(), it);
        size_t nextIndex = (index + 1) % tabs_.size();
        tabs_[nextIndex]->focus();
    }
}

void TabManager::prevTab() {
    if (tabs_.empty()) return;

    // Find current tab
    Tab* current = getCurrentTab();
    if (!current) return;

    // Find index of current tab
    auto it = std::find_if(tabs_.begin(), tabs_.end(),
                          [current](const std::unique_ptr<Tab>& tab) {
                              return tab.get() == current;
                          });

    if (it != tabs_.end()) {
        // Move to previous tab or wrap around
        size_t index = std::distance(tabs_.begin(), it);
        size_t prevIndex = (index == 0) ? tabs_.size() - 1 : index - 1;
        tabs_[prevIndex]->focus();
    }
}

Tab* TabManager::getCurrentTab() {
    if (tabs_.empty()) return nullptr;

    const gchar* currentName = gtk_stack_get_visible_child_name(stack_);
    if (!currentName) return nullptr;

    std::string currentId(currentName);

    auto it = std::find_if(tabs_.begin(), tabs_.end(),
                          [&currentId](const std::unique_ptr<Tab>& tab) {
                              return tab->getId() == currentId;
                          });

    return (it != tabs_.end()) ? it->get() : nullptr;
}
