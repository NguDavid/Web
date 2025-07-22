#ifndef EPOLLMULTIPLEXER_HPP
    #define EPOLLMULTIPLEXER_HPP

#include <unordered_map>
#include <sys/epoll.h>
#include <vector>

#include "IMultiplexer.hpp"
#include "ClientSocket.hpp"

inline constexpr int MAX_EVENTS = 64;
inline constexpr int BUFFER_SIZE = 4096;

class EpollMultiplexer : public IMultiplexer {
    public:
        EpollMultiplexer();
        EpollMultiplexer(const EpollMultiplexer &other) = default;
        EpollMultiplexer& operator=(const EpollMultiplexer &other) = default;
        ~EpollMultiplexer();

        void addSocket(ClientSocket *client) override;
        void removeSocket(ClientSocket *client) override;
        void handleEvents(ServerSocket &serverSock) override;
        void addServerSocket(ServerSocket &serverSock);

    private:
        int epollFd;
        std::unordered_map<int, ClientSocket*> clients;
        std::vector<epoll_event> events = std::vector<epoll_event>(MAX_EVENTS);
};

#endif /* EPOLLMULTIPLEXER_HPP */
