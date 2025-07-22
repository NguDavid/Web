#include <iostream>
#include <fcntl.h>

#include "Server.hpp"

Server::Server(ServerSocket &serverSocket, IMultiplexer &multiplexer) : serverSocket(serverSocket), multiplexer(multiplexer) {}

void Server::run()
{
    while (true) {
        multiplexer.handleEvents(serverSocket);
    }
}
