#ifndef SERVER_HPP
    #define SERVER_HPP

#include "ModuleManager.hpp"
#include "ServerSocket.hpp"
#include "IMultiplexer.hpp"

class Server {
public:
    Server(ServerSocket &serverSocket, IMultiplexer &multiplexer, ModuleManager &moduleManager);
    void run();

private:
    ServerSocket &serverSocket;
    IMultiplexer &multiplexer;
    ModuleManager &moduleManager;
};

#endif /* SERVER_HPP */
