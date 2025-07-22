#ifndef SERVER_HPP
    #define SERVER_HPP

#include "ServerSocket.hpp"
#include "IMultiplexer.hpp"

class Server {
    public:
        Server(ServerSocket &serverSocket, IMultiplexer &multiplexer);
        void run();

    private:
        ServerSocket &serverSocket;
        IMultiplexer &multiplexer;
};

#endif /* SERVER_HPP */
