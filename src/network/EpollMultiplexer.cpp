#include <unistd.h>
#include <iostream>
#include <cstring>

#include "ModuleManager.hpp"
#include "EpollMultiplexer.hpp"
#include "HttpParser.hpp"

EpollMultiplexer::EpollMultiplexer()
{
    epollFd = epoll_create1(0);
    if (epollFd == -1) {
        std::cout << "[MULTIPLEXER] Error => set epoll." << std::endl;
    }
}

EpollMultiplexer::~EpollMultiplexer()
{
    close(epollFd);
}

void EpollMultiplexer::addSocket(ClientSocket *client)
{
    epoll_event ev;

    std::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = client->getFd();
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, client->getFd(), &ev) == -1) {
        std::cout << "[MULTIPLEXER] Error => add socket." << std::endl;
        return;
    }
    clients[client->getFd()] = client;
}

void EpollMultiplexer::removeSocket(ClientSocket *client)
{
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, client->getFd(), nullptr) == -1) {
        std::cout << "[MULTIPLEXER] Error => delete socket." << std::endl;
    }
    clients.erase(client->getFd());
}
/*
void EpollMultiplexer::handleEvents(ServerSocket &serverSock, ModuleManager &moduleManager)
{
    int nfds = epoll_wait(epollFd, events.data(), MAX_EVENTS, -1);
    if (nfds == -1) {
        std::cout << "[MULTIPLEXER] Error => epoll wait." << std::endl;
        return;
    }
    for (int i = 0; i < nfds; i++) {
        int fd = events[i].data.fd;

        if (fd == serverSock.getFd()) {
            ClientSocket *client = serverSock.acceptClient();
            if (client) {
                addSocket(client);
                std::cout << "[SERVER] New client connected." << std::endl;
            }
        } else {
            auto it = clients.find(fd);
            if (it != clients.end()) {
                ClientSocket *client = it->second;
                std::string buffer;
                int bytesRead = client->receive(buffer);
                if (bytesRead > 0) {
                    std::cout << "[SERVER] Received:\n" << buffer << std::endl;
                    HttpParser parser;
                    if (!parser.parse(buffer)) {
                        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 11\r\n\r\nBad Request";
                        client->send(response);
                        removeSocket(client);
                        close(fd);
                        delete client;
                        continue;
                    }
                    const auto &parsedData = parser.get_data();
                    const std::string &body = parser.get_body();
                    IModule *module = moduleManager.findModuleFor(parsedData);
                    if (module) {
                        std::string response = module->handleRequest(parsedData, body);
                        client->send(response);
                    } else {
                        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
                        client->send(response);
                    }
                } else {
                    std::cout << "[SERVER] Client disconnected." << std::endl;
                    removeSocket(client);
                    close(fd);
                    delete client;
                }
            }
        }
    }
}
*/
void EpollMultiplexer::handleEvents(ServerSocket &serverSock, ModuleManager &moduleManager)
{
    int nfds = epoll_wait(epollFd, events.data(), MAX_EVENTS, -1);
    int fd = 0;
    int i = 0;

    if (nfds == -1) {
        std::cerr << "[MULTIPLEXER] Error => epoll wait." << std::endl;
        return;
    }
    while (i < nfds) {
        fd = events[i].data.fd;
        if (fd == serverSock.getFd()) {
            handleNewConnection(serverSock);
        } else {
            auto it = clients.find(fd);
            if (it != clients.end())
                handleClientData(it->second, moduleManager);
        }
        ++i;
    }
}

void EpollMultiplexer::handleNewConnection(ServerSocket &serverSock)
{
    ClientSocket *client = serverSock.acceptClient();

    if (client) {
        addSocket(client);
        std::cout << "[SERVER] New client connected." << std::endl;
    }
}

void EpollMultiplexer::handleClientData(ClientSocket *client, ModuleManager &moduleManager)
{
    HttpParser parser;
    std::string buffer;
    int bytesRead = client->receive(buffer);

    if (bytesRead <= 0) {
        std::cout << "[SERVER] Client disconnected." << std::endl;
        cleanupClient(client);
        return;
    }
    std::cout << "[SERVER] Received: " << buffer << std::endl;
    if (!parser.parse(buffer)) {
        std::string response = "HTTP/1.1 400 Bad Request\nContent-Length: 11\n\nBad Request";
        client->send(response);
        cleanupClient(client);
        return;
    }
    const auto &parsedData = parser.get_data();
    const std::string &body = parser.get_body();
    IModule *module = moduleManager.findModuleFor(parsedData);
    if (module) {
        std::string response = module->handleRequest(parsedData, body);
        client->send(response);
    } else {
        std::string response = "HTTP/1.1 404 Not Found\nContent-Length: 13\n\n404 Not Found";
        client->send(response);
    }
}

void EpollMultiplexer::cleanupClient(ClientSocket *client)
{
    int fd = client->getFd();

    removeSocket(client);
    close(fd);
    delete client;
}

void EpollMultiplexer::addServerSocket(ServerSocket &serverSock)
{
    epoll_event ev;

    std::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = serverSock.getFd();
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSock.getFd(), &ev) == -1) {
        std::cout << "[MULTIPLEXER] Error => add server socket to epoll." << std::endl;
    } else {
        std::cout << "[MULTIPLEXER] Server socket added to epoll." << std::endl;
    }
}
