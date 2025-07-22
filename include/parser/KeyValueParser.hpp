#ifndef KEYVALUEPARSER_HPP
    #define KEYVALUEPARSER_HPP

#include <string>
#include <vector>
#include <unordered_map>

class KeyValueParser {
    public:
        KeyValueParser();
        ~KeyValueParser();
        KeyValueParser(const KeyValueParser&);
        KeyValueParser& operator=(const KeyValueParser&);

        const std::unordered_map<std::string, std::string> &get_data() const;
        bool checkRequiredKeys() const;
        std::string trim(const std::string&) const;

    protected:
        std::unordered_map<std::string, std::string> data;
        std::vector<std::string> requiredKeys;
};

#endif /* KEYVALUEPARSER_HPP */
