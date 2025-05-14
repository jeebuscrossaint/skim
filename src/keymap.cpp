#include "keymap.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

void KeyMap::registerKey(guint keyval, GdkModifierType state, KeyCallback callback) {
    KeyCombo combo{keyval, state};
    keyBindings[combo] = callback;
}

bool KeyMap::registerKey(const std::string& keySpec, KeyCallback callback) {
    std::istringstream stream(keySpec);
    std::string token;
    std::vector<std::string> tokens;

    // Split by '+' character
    while (std::getline(stream, token, '+')) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        std::cerr << "Invalid key specification: " << keySpec << std::endl;
        return false;
    }

    // The last token is the key
    std::string keyName = tokens.back();
    tokens.pop_back();

    // Convert key name to keyval
    guint keyval = gdk_keyval_from_name(keyName.c_str());
    if (keyval == GDK_KEY_VoidSymbol) {
        std::cerr << "Unknown key name: " << keyName << std::endl;
        return false;
    }

    // Parse modifiers
    GdkModifierType state = static_cast<GdkModifierType>(0);
    for (const auto& mod : tokens) {
        if (mod == "ctrl" || mod == "control") {
            state = static_cast<GdkModifierType>(state | GDK_CONTROL_MASK);
        } else if (mod == "shift") {
            state = static_cast<GdkModifierType>(state | GDK_SHIFT_MASK);
        } else if (mod == "alt") {
            state = static_cast<GdkModifierType>(state | GDK_MOD1_MASK);
        } else if (mod == "super" || mod == "meta" || mod == "win") {
            state = static_cast<GdkModifierType>(state | GDK_SUPER_MASK);
        } else {
            std::cerr << "Unknown modifier: " << mod << std::endl;
            return false;
        }
    }

    // Register the key binding
    registerKey(keyval, state, callback);
    return true;
}

bool KeyMap::processKeyEvent(GdkEventKey* event) {
    // Extract keyval and state from the event
    guint keyval = event->keyval;
    GdkModifierType state = static_cast<GdkModifierType>(event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK | GDK_SUPER_MASK));

    KeyCombo combo{keyval, state};

    // Look up the key binding
    auto it = keyBindings.find(combo);
    if (it != keyBindings.end()) {
        // Call the callback function
        return it->second();
    }

    return false;
}
