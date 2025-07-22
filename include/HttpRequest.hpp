#ifndef HTTPREQUEST_HPP
    #define HTTPREQUEST_HPP

#include <unordered_map>
#include <string>

class HttpRequest {
    public:
        HttpRequest();
        HttpRequest(const HttpRequest &other);
        HttpRequest& operator=(const HttpRequest &other);
        ~HttpRequest();

        bool parse(const std::string &request);
        const std::string &getMethod() const;
        const std::string &getPath() const;
        const std::string &getVersion() const;
        const std::unordered_map<std::string, std::string> &getHeaders() const;
        const std::string &getBody() const;

    private:
        std::string method;
        std::string path;
        std::string version;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
};

#endif /* HTTPREQUEST_HPP */
