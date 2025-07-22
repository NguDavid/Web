#include <sstream>
#include <iostream>
#include <algorithm>

#include "KeyValueParser.hpp"

KeyValueParser::KeyValueParser() = default;

KeyValueParser::~KeyValueParser() = default;

KeyValueParser::KeyValueParser(const KeyValueParser&) = default;

KeyValueParser& KeyValueParser::operator=(const KeyValueParser&) = default;

bool KeyValueParser::checkRequiredKeys() const
{
    auto it = requiredKeys.begin();

    while (it != requiredKeys.end()) {
        if (data.find(*it) == data.end())
            return (false);
        it += 1;
    }
    return (true);
}

std::string KeyValueParser::trim(const std::string &str) const
{
    const auto start = str.find_first_not_of(" \t\r\n");

    if (start == std::string::npos)
        return ("");
    const auto end = str.find_last_not_of(" \t\r\n");
    return (str.substr(start, end - start + 1));
}

const std::unordered_map<std::string, std::string> &KeyValueParser::get_data() const
{
    return (data);
}
