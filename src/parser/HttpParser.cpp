#include <sstream>
#include <iostream>
#include <algorithm>

#include "HttpParser.hpp"

HttpParser::HttpParser()
{
    requiredKeys = {
        "Content-Type",
        "Content-Length"
    };
}

HttpParser::~HttpParser() = default;

HttpParser::HttpParser(const HttpParser&) = default;

HttpParser& HttpParser::operator=(const HttpParser&) = default;

bool HttpParser::parse(const std::string &headers)
{
    std::unordered_map<std::string, std::string> tempData;
    std::istringstream stream(headers);
    std::string line;
    bool inBody = false;
    std::ostringstream bodyStream;

    while (std::getline(stream, line)) {
        if (inBody) {
            bodyStream << line << "\n";
            continue;
        }
        if (line.empty()) {
            inBody = true;
            continue;
        }
        line = trim(line);
        if (line.empty())
            continue;
        if (!parse_line(line, tempData)) {
            data.clear();
            return (false);
        }
    }
    data = std::move(tempData);
    body = bodyStream.str();
    return (checkRequiredKeys());
}

bool HttpParser::parse_line(const std::string &line, std::unordered_map<std::string, std::string> &tempData)
{
    auto pos = line.find(':');
    std::string key;
    std::string value;

    if (pos == std::string::npos)
        return (true);
    key = trim(line.substr(0, pos));
    value = trim(line.substr(pos + 1));
    if (tempData.find(key) != tempData.end())
        return (false);
    tempData[key] = value;
    return (true);
}

const std::string &HttpParser::get_body() const
{
    return (body);
}
