#include "config.hpp"
#include <filesystem>
#include <iostream>

Config::Config() : L(nullptr) {
    // Initialize Lua
    L = luaL_newstate();
    luaL_openlibs(L);

    // Set default values
    setValue("homepage", "https://duckduckgo.com");
    setValue("searchEngine", "https://duckduckgo.com/?q=%s");
    setValue("width", 1024);
    setValue("height", 768);
    setValue("enableJavaScript", true);
    setValue("enableSmoothScrolling", true);
    setValue("enableDeveloperExtras", true);
    setValue("enableWebGL", true);
    setValue("darkMode", false);
}

Config::~Config() {
    if (L) {
        lua_close(L);
    }
}

bool Config::load() {
    // Look for config in common locations
    std::string homePath = std::getenv("HOME") ? std::getenv("HOME") : "";
    if (homePath.empty()) {
        homePath = std::getenv("USERPROFILE") ? std::getenv("USERPROFILE") : "";
    }

    if (homePath.empty()) {
        std::cerr << "Could not determine home directory" << std::endl;
        return false;
    }

    std::string configPath = homePath + "/.config/skim/config.lua";

    if (!std::filesystem::exists(configPath)) {
        // Config doesn't exist, use defaults
        std::cout << "No config file found at " << configPath << ". Using defaults." << std::endl;
        return true;
    }

    // Load the config file
    if (luaL_dofile(L, configPath.c_str()) != 0) {
        std::cerr << "Error loading config: " << lua_tostring(L, -1) << std::endl;
        return false;
    }

    // Extract values from lua into our settings map
    lua_getglobal(L, "skim");
    if (lua_istable(L, -1)) {
        // Process config values
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            std::string key = lua_tostring(L, -2);

            if (lua_isstring(L, -1)) {
                setValue(key, std::string(lua_tostring(L, -1)));
            } else if (lua_isboolean(L, -1)) {
                setValue(key, lua_toboolean(L, -1) != 0);
            } else if (lua_isnumber(L, -1)) {
                setValue(key, (int)lua_tonumber(L, -1));
            }

            lua_pop(L, 1);
        }
    }

    return true;
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) {
    try {
        return std::any_cast<std::string>(settings[key]);
    } catch (...) {
        return defaultValue;
    }
}

bool Config::getBool(const std::string& key, bool defaultValue) {
    try {
        return std::any_cast<bool>(settings[key]);
    } catch (...) {
        return defaultValue;
    }
}

int Config::getInt(const std::string& key, int defaultValue) {
    try {
        return std::any_cast<int>(settings[key]);
    } catch (...) {
        return defaultValue;
    }
}

void Config::setValue(const std::string& key, const std::any& value) {
    settings[key] = value;
}
