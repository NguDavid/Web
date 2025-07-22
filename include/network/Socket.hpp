#ifndef SOCKET_HPP
    #define SOCKET_HPP

#include <netinet/in.h>
#include <unistd.h>

inline constexpr int BUFFER_SIZE = 4096;

class Socket {
    public:
        Socket();
        Socket(const Socket &other) = default;
        Socket &operator=(const Socket &other) = default;
        ~Socket();

        int getFd() const;
        void closeFd();

    protected:
        int fd;
};

#endif /* SOCKET_HPP */
