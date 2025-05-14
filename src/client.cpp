#include "client.hpp"
#include <random>
#include <algorithm>

int Client::generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 1000000);
    return dis(gen);
}

Client::Client() : id_(generateId()) {
    window_ = std::make_unique<Window>();
}

Client::~Client() {
    // Window is automatically cleaned up by unique_ptr
}

ClientManager::ClientManager() {
    // Nothing to initialize
}

ClientManager::~ClientManager() {
    // Clients are automatically cleaned up by shared_ptr
}

std::shared_ptr<Client> ClientManager::createClient() {
    auto client = std::make_shared<Client>();
    clients_.push_back(client);
    return client;
}

std::shared_ptr<Client> ClientManager::getClientById(int id) {
    auto it = std::find_if(clients_.begin(), clients_.end(),
                          [id](const std::shared_ptr<Client>& client) {
                              return client->getId() == id;
                          });

    if (it != clients_.end()) {
        return *it;
    }

    return nullptr;
}

void ClientManager::destroyClient(int id) {
    auto it = std::find_if(clients_.begin(), clients_.end(),
                          [id](const std::shared_ptr<Client>& client) {
                              return client->getId() == id;
                          });

    if (it != clients_.end()) {
        clients_.erase(it);

        // If no more clients, exit the application
        if (clients_.empty()) {
            gtk_main_quit();
        }
    }
}
