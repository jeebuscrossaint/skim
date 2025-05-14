#pragma once

#include <memory>
#include <vector>
#include "window.hpp"
#include "webview.hpp"

class Client {
public:
    Client();
    ~Client();

    Window& getWindow() { return *window_; }
    int getId() const { return id_; }

private:
    std::unique_ptr<Window> window_;
    int id_;

    static int generateId();
};

// Manages all browser clients
class ClientManager {
public:
    ClientManager();
    ~ClientManager();

    std::shared_ptr<Client> createClient();
    std::shared_ptr<Client> getClientById(int id);
    void destroyClient(int id);

private:
    std::vector<std::shared_ptr<Client>> clients_;
};
