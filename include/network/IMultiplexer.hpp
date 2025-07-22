#ifndef IMULTIPLEXER_HPP
    #define IMULTIPLEXER_HPP

#include "ModuleManager.hpp"
#include "ClientSocket.hpp"
#include "ServerSocket.hpp"

class IMultiplexer {
public:
    virtual ~IMultiplexer() {}

    virtual void addSocket(ClientSocket *socket) = 0;
    virtual void removeSocket(ClientSocket *socket) = 0;
    virtual void handleEvents(ServerSocket &serverSock, ModuleManager &moduleManager) = 0;
    virtual void addServerSocket(ServerSocket &serverSock) = 0;
};

#endif /* IMULTIPLEXER_HPP */
