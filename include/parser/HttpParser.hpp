#ifndef HTTPPARSER_HPP
    #define HTTPPARSER_HPP

#include <unordered_map>
#include <string>
#include <vector>

#include "KeyValueParser.hpp"

class HttpParser : public KeyValueParser {
    public:
        HttpParser();
        ~HttpParser();
        HttpParser(const HttpParser&);
        HttpParser& operator=(const HttpParser&);

        bool parse(const std::string&);

        bool parse_line(const std::string&, std::unordered_map<std::string, std::string>&);
        const std::string &get_body() const;

    private:
        std::string body;
};

#endif /* HTTPPARSER_HPP */
