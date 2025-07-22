#ifndef IMODULE_HPP
    #define IMODULE_HPP

#include <string>

#include "HttpRequest.hpp"

class IModule {
    public:
        virtual ~IModule();

        virtual std::string handleRequest(const HttpRequest &request) = 0;
};

#endif /* IMODULE_HPP */
