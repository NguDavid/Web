/*
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
*/
#include <iostream>

#include "EpollMultiplexer.hpp"
#include "IniParser.hpp"
#include "Server.hpp"
#include "ModuleManager.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    IniFileParser iniParser;
    if (!iniParser.parse(argv[1])) {
        std::cout << "[ERROR] Failed to parse config file: " << argv[1] << std::endl;
        return 1;
    }
    auto config = iniParser.get_data();

    ServerSocket server_socket;
    if (!server_socket.setServerSocket(config)) {
        std::cerr << "[ERROR] Failed to bind or listen." << std::endl;
        return 1;
    }

    EpollMultiplexer multiplexer;
    multiplexer.addServerSocket(server_socket);

    ModuleManager moduleManager;
    moduleManager.loadModules(config);

    Server server(server_socket, multiplexer, moduleManager);

    std::cout << "[INFO] Server is running on port " << config["server.port"] << ".\n";
    server.run();

    return 0;
}

