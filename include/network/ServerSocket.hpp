#ifndef SERVERSOCKET_HPP
    #define SERVERSOCKET_HPP

#include <unordered_map>
#include <netinet/in.h>
#include <string>

#include "ClientSocket.hpp"
#include "Socket.hpp"

class ServerSocket : public Socket {
    public:
        ServerSocket();
        ServerSocket(const ServerSocket &other) = default;
        ServerSocket& operator=(const ServerSocket &other) = default;

        bool setServerSocket(const std::unordered_map<std::string, std::string> &config);
        bool bind(const std::string &port);
        bool listen(int max_clients);
        ClientSocket *acceptClient();
        struct sockaddr_in getClientAddr() const;

    private:
        struct sockaddr_in serverAddr;
};

#endif /* SERVERSOCKET_HPP */
