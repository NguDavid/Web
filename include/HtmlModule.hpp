#ifndef HTMLMODULE_HPP
    #define HTMLMODULE_HPP

#include <string>
#include <fstream>
#include <sstream>

#include "IModule.hpp"

class HtmlModule : public IModule {
    public:
        HtmlModule();
        HtmlModule(const HtmlModule &other);
        HtmlModule& operator=(const HtmlModule &other);
        ~HtmlModule();

        std::string handleRequest(const std::unordered_map<std::string, std::string> &parsedData, const std::string &body) override;
        bool canHandle(const std::unordered_map<std::string, std::string> &parsedData) const override;

    private:
        std::string readFile(const std::string &filepath) const;
};

extern "C" {
    IModule *createModule();
}

#endif /* HTMLMODULE_HPP */
