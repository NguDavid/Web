#ifndef INIPARSER_HPP
    #define INIPARSER_HPP

#include <unordered_map>
#include <string>
#include <vector>

#include "KeyValueParser.hpp"

class IniFileParser : public KeyValueParser {
    public:
        IniFileParser();
        ~IniFileParser();
        IniFileParser(const IniFileParser&);
        IniFileParser& operator=(const IniFileParser&);

        bool parse(const std::string &filepath);

        bool parse_line(const std::string&, std::unordered_map<std::string, std::string>&);

    private:
        std::string currentSection;
};

#endif /* INIPARSER_HPP */
