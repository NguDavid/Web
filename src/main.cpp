#include <iostream>

#include "EpollMultiplexer.hpp"
#include "IniParser.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
    ServerSocket server_socket;
    EpollMultiplexer multiplexer;
    IniFileParser iniParser;

    if (argc != 2)
        return (1);
    if (!iniParser.parse(argv[1])) {
        std::cout << "[ERROR] Failed to parse config file: " << argv[1] << std::endl;
        return (1);
    }
    auto config = iniParser.get_data();
    server_socket.setServerSocket(config);
    multiplexer.addServerSocket(server_socket);
    Server server(server_socket, multiplexer);
    std::cout << "[INFO] Server is running on port " << argv[1] << ".\n";
    server.run();
    return (0);
}
