#ifndef IMULTIPLEXER_HPP
    #define IMULTIPLEXER_HPP

#include "ClientSocket.hpp"
#include "ServerSocket.hpp"

class IMultiplexer {
public:
    virtual ~IMultiplexer() {}

    virtual void addSocket(ClientSocket *socket) = 0;
    virtual void removeSocket(ClientSocket *socket) = 0;
    virtual void handleEvents(ServerSocket &serverSocket) = 0;
};

#endif /* IMULTIPLEXER_HPP */
