#include <unistd.h>
#include <iostream>
#include <cstring>

#include "EpollMultiplexer.hpp"

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

void EpollMultiplexer::handleEvents(ServerSocket &serverSock)
{
    int nfds = epoll_wait(epollFd, events.data(), MAX_EVENTS, -1);
    int i = 0;
    int fd = 0;
    char buffer[BUFFER_SIZE];
    int bytes = 0;

    if (nfds == -1) {
        std::cout << "[MULTIPLEXER] Error => epoll wait." << std::endl;
        return;
    }
    while (i < nfds) {
        if (events[i].data.fd == serverSock.getFd()) {
            ClientSocket *client = serverSock.acceptClient();
            if (client) {
                addSocket(client);
                std::cout << "[SERVER] New client." << std::endl;
            }
        } else {
            fd = events[i].data.fd;
            auto it = clients.find(fd);
            if (it != clients.end()) {
                ClientSocket *client = it->second;
                std::memset(buffer, 0, sizeof(buffer));
                bytes = read(fd, buffer, sizeof(buffer));
                if (bytes > 0) {
                    std::cout << "[SERVER] Received: " << std::string(buffer, bytes) << std::endl;
                } else {
                    removeSocket(client);
                    close(fd);
                    std::cout << "[SERVER] Disconnect client." << std::endl;
                    delete (client);
                }
            }
        }
        i += 1;;
    }
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
