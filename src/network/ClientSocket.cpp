#include <unistd.h>
#include <cstring>

#include "ClientSocket.hpp"

ClientSocket::ClientSocket() {}

int ClientSocket::receive(std::string &buffer)
{
    char temp[4096];
    int bytesRead = read(fd, temp, sizeof(temp));

    if (bytesRead > 0)
        buffer.assign(temp, bytesRead);
    return (bytesRead);
}

int ClientSocket::send(const std::string &data)
{
    return (write(fd, data.c_str(), data.length()));
}
