#include "Socket.hpp"

Socket::Socket() : fd(-1) {}

Socket::~Socket()
{
    closeFd();
}

int Socket::getFd() const
{
    return (fd);
}

void Socket::closeFd()
{
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}
