#pragma once

#include <string>
#include <map>
#include <any>
#include <memory>
#include <lua.hpp>

class Config {
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    bool load();

    std::string getString(const std::string& key, const std::string& defaultValue = "");
    bool getBool(const std::string& key, bool defaultValue = false);
    int getInt(const std::string& key, int defaultValue = 0);

    void setValue(const std::string& key, const std::any& value);

    lua_State* getLuaState() { return L; }

private:
    Config();
    ~Config();

    // Disable copy
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    std::map<std::string, std::any> settings;
    lua_State* L;
};
