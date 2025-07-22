#ifndef CLIENTSOCKET_HPP
    #define CLIENTSOCKET_HPP

#include <string>

#include "Socket.hpp"

class ClientSocket : public Socket {
    public:
        ClientSocket();
        ClientSocket(int fd) { this->fd = fd; }
        ClientSocket(const ClientSocket &other) = default;
        ClientSocket& operator=(const ClientSocket &other) = default;

        int receive(std::string &buffer);
        int send(const std::string &data);
};

#endif /* CLIENTSOCKET_HPP */
