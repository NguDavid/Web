#ifndef IMODULE_HPP
    #define IMODULE_HPP

#include <unordered_map>
#include <string>

class IModule {
    public:
        virtual ~IModule() = default;

        virtual std::string handleRequest(const std::unordered_map<std::string, std::string> &parsedData, const std::string &body) = 0;
        virtual bool canHandle(const std::unordered_map<std::string, std::string> &parsedData) const = 0;
};

#endif /* IMODULE_HPP */
