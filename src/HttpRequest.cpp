#include "HttpRequest.hpp"
#include <sstream>
#include <algorithm>

HttpRequest::HttpRequest() = default;

HttpRequest::HttpRequest(const HttpRequest &other)
{
    *this = other;
}

HttpRequest& HttpRequest::operator=(const HttpRequest &other)
{
    if (this != &other) {
        method = other.method;
        path = other.path;
        version = other.version;
        headers = other.headers;
        body = other.body;
    }
    return (*this);
}

HttpRequest::~HttpRequest() = default;

bool HttpRequest::parse(const std::string &request)
{
    std::istringstream stream(request);
    std::string line;

    if (!std::getline(stream, line))
        return (false);
    std::istringstream req_line(line);
    if (!(req_line >> method >> path >> version))
        return (false);
    while (std::getline(stream, line)) {
        if (line == "\n" || line.empty())
            break;
        int pos = line.find(':');
        if (pos == std::string::npos)
            continue;
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
        value.erase(0, value.find_first_not_of(" \t\r"));
        value.erase(value.find_last_not_of(" \t\r") + 1);
        headers[key] = value;
    }
    std::ostringstream bodyStream;
    while (std::getline(stream, line)) {
        bodyStream << line << "\n";
    }
    body = bodyStream.str();
    return (true);
}

const std::string &HttpRequest::getMethod() const
{
    return (method);
}

const std::string &HttpRequest::getPath() const
{
    return (path);
}

const std::string &HttpRequest::getVersion() const
{
    return (version);
}

const std::unordered_map<std::string, std::string> &HttpRequest::getHeaders() const
{
    return (headers);
}

const std::string &HttpRequest::getBody() const
{
    return (body);
}
