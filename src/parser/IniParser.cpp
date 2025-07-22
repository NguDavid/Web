#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "IniParser.hpp"

#warning Ajouter les modules.
IniFileParser::IniFileParser()
{
    requiredKeys = {
        "server.port",
        "server.host"
    };
}

IniFileParser::~IniFileParser() = default;

IniFileParser::IniFileParser(const IniFileParser&) = default;

IniFileParser& IniFileParser::operator=(const IniFileParser&) = default;

bool IniFileParser::parse(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::unordered_map<std::string, std::string> tempData;
    std::string line;

    if (!file.is_open())
        return (false);
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';')
            continue;
        if (line.front() == '[' && line.back() == ']') {
            currentSection = trim(line.substr(1, line.length() - 2));
            continue;
        }
        if (!parse_line(line, tempData)) {
            data.clear();
            return (false);
        }
    }
    data = std::move(tempData);
    return (checkRequiredKeys());
}

bool IniFileParser::parse_line(const std::string &line, std::unordered_map<std::string, std::string> &tempData)
{
    auto pos = line.find('=');
    std::string key;
    std::string value;

    if (pos == std::string::npos)
        return (true);
    key = trim(line.substr(0, pos));
    if (!currentSection.empty())
        key = currentSection + "." + key;
    value = trim(line.substr(pos + 1));
    auto commentPos = value.find_first_of(";#");
    if (commentPos != std::string::npos)
        value = trim(value.substr(0, commentPos));
    if (tempData.find(key) != tempData.end())
        return (false);
    tempData[key] = value;
    return (true);
}
