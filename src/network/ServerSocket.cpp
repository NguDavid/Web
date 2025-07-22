#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "EpollMultiplexer.hpp"
#include "ServerSocket.hpp"

ServerSocket::ServerSocket()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
}

bool ServerSocket::setServerSocket(const std::unordered_map<std::string, std::string> &config)
{
    auto itPort = config.find("server.port");

    if (itPort != config.end()) {
        if (!bind(itPort->second))
            return (false);
    } else {
        return (false);
    }
    if (!listen(MAX_EVENTS))
        return (false);
    std::cout << "[ServerSocket] Bound and listening on " << itPort->second << std::endl;
    return (true);
}

bool ServerSocket::bind(const std::string &port)
{
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(std::atoi(port.c_str()));
    return (::bind(fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != -1);
}

bool ServerSocket::listen(int maxClients)
{
    return (::listen(fd, maxClients) != -1);
}

ClientSocket *ServerSocket::acceptClient()
{
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    int clientFd = accept(fd, (struct sockaddr *)&clientAddr, &len);

    if (clientFd < 0)
        return (nullptr);
    return (new ClientSocket(clientFd));
}

struct sockaddr_in ServerSocket::getClientAddr() const
{
    return (serverAddr);
}
