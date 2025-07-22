#include "HtmlModule.hpp"

HtmlModule::HtmlModule() = default;

HtmlModule::~HtmlModule() = default;

HtmlModule::HtmlModule(const HtmlModule &other) = default;

HtmlModule& HtmlModule::operator=(const HtmlModule &other) = default;

std::string HtmlModule::handleRequest(const HttpRequest& request)
{
    std::string filepath = request.getPath();
    return (readFile(filepath));
}

std::string HtmlModule::readFile(const std::string& filepath) const
{
    std::ifstream file(filepath.c_str());
    std::ostringstream content;
    std::string line;

    if (!file.is_open())
        return ("version: HTTP/1.1\ncode: 404\nmessage: Not Found\nContent-Length: 0\n\n");
    while (std::getline(file, line)) {
        content << line << "\n";
    }
    file.close();
    std::string body = content.str();
    std::ostringstream response;
    response << "version: HTTP/1.1\ncode: 200\nmessage: OK\n";
    response << "Content-Length: " << body.size() << "\n";
    response << "Content-Type: text/html\n";
    response << "\n";
    response << body;
    return (response.str());
}

bool HtmlModule::canHandle(const HttpRequest &req) const
{
    return (req.getPath().size() >= 5 &&
            req.getPath().substr(req.getPath().size() - 5) == ".html");
}

extern "C" IModule* createModule()
{
    return (new HtmlModule());
}
