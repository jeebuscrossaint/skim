#pragma once

#include <functional>
#include <map>
#include <string>
#include <gtk/gtk.h>

class KeyMap {
public:
    using KeyCallback = std::function<bool()>;

    static KeyMap& getInstance() {
        static KeyMap instance;
        return instance;
    }

    // Register a new key binding
    void registerKey(guint keyval, GdkModifierType state, KeyCallback callback);

    // Register a key by string representation (e.g., "ctrl+l")
    bool registerKey(const std::string& keySpec, KeyCallback callback);

    // Process key event
    bool processKeyEvent(GdkEventKey* event);

private:
    KeyMap() = default;
    ~KeyMap() = default;

    // Disable copy
    KeyMap(const KeyMap&) = delete;
    KeyMap& operator=(const KeyMap&) = delete;

    struct KeyCombo {
        guint keyval;
        GdkModifierType state;

        bool operator<(const KeyCombo& other) const {
            return (keyval < other.keyval) || (keyval == other.keyval && state < other.state);
        }
    };

    std::map<KeyCombo, KeyCallback> keyBindings;
};
