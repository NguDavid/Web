#include <iostream>
#include <fcntl.h>

#include "Server.hpp"

Server::Server(ServerSocket &serverSocket, IMultiplexer &multiplexer, ModuleManager &moduleManager)
    : serverSocket(serverSocket), multiplexer(multiplexer), moduleManager(moduleManager) {}

void Server::run()
{
    while (true) {
        multiplexer.handleEvents(serverSocket, moduleManager);
    }
}
