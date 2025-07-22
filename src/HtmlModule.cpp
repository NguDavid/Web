#include "HtmlModule.hpp"

HtmlModule::HtmlModule() = default;

HtmlModule::~HtmlModule() = default;

HtmlModule::HtmlModule(const HtmlModule &other) = default;

HtmlModule& HtmlModule::operator=(const HtmlModule &other) = default;

std::string HtmlModule::handleRequest(const std::unordered_map<std::string, std::string> &parsedData, const std::string &)
{
    auto it = parsedData.find("Path");

    if (it == parsedData.end())
        return ("Version: HTTP/1.1\nCode: 400\nMessage: Bad Request\nContent-Length: 0\n\n");
    std::string filepath = it->second;
    return (readFile(filepath));
}

std::string HtmlModule::readFile(const std::string& filepath) const
{
    std::ifstream file(filepath.c_str());
    std::ostringstream content;
    std::string line;

    if (!file.is_open())
        return ("Version: HTTP/1.1\nCode: 404\nMessage: Not Found\nContent-Length: 0\n\n");
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

bool HtmlModule::canHandle(const std::unordered_map<std::string, std::string> &parsedData) const
{
    auto it = parsedData.find("Path");

    if (it == parsedData.end())
        return (false);
    const std::string &path = it->second;
    return (path.size() >= 5 && path.substr(path.size() - 5) == ".html");
}

extern "C" IModule *createModule()
{
    return (new HtmlModule());
}
