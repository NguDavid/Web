#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <fstream>
#include <cstdio>
#include <string>

#include "IniParser.hpp"
#include "HttpParser.hpp"

static void write_temp_ini(const char* filename, const char* content)
{
    std::ofstream file(filename);
    file << content;
    file.close();
}

Test(IniFileParser, parse_valid_file)
{
    const char* filename = "test_valid.ini";
    const char* content = 
        "# Comment line\n"
        "key1 = value1\n"
        "key2=value2  ; comment\n"
        "key3=    value3\n";

    write_temp_ini(filename, content);

    IniFileParser parser;
    bool res = parser.parse(filename);

    std::remove(filename);

    cr_assert(res == true);
    auto& data = parser.get_data();
    cr_assert(data.at("key1") == "value1");
    cr_assert(data.at("key2") == "value2");
    cr_assert(data.at("key3") == "value3");
}

Test(IniFileParser, parse_file_with_duplicate_key)
{
    const char* filename = "test_duplicate.ini";
    const char* content = 
        "key1=value1\n"
        "key1=value2\n";

    write_temp_ini(filename, content);

    IniFileParser parser;
    bool res = parser.parse(filename);

    std::remove(filename);

    cr_assert(res == false);
    cr_assert(parser.get_data().empty());
}

Test(IniFileParser, parse_file_not_exist)
{
    IniFileParser parser;
    bool res = parser.parse("non_existent_file.ini");
    cr_assert(res == false);
    cr_assert(parser.get_data().empty());
}

Test(HttpParser, parse_valid_headers)
{
    HttpParser parser;
    std::string headers = 
        "Host: example.com\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 123\n";

    bool res = parser.parse(headers);
    cr_assert(res == true);

    auto& data = parser.get_data();
    cr_assert(data.at("Host") == "example.com");
    cr_assert(data.at("Content-Type") == "text/html");
    cr_assert(data.at("Content-Length") == "123");
}

Test(HttpParser, parse_headers_with_duplicate_key)
{
    HttpParser parser;
    std::string headers = 
        "Host: example.com\n"
        "Host: example.org\n";

    bool res = parser.parse(headers);
    cr_assert(res == false);
    cr_assert(parser.get_data().empty());
}

Test(HttpParser, parse_empty_string)
{
    HttpParser parser;
    bool res = parser.parse("");
    cr_assert(res == true);
    cr_assert(parser.get_data().empty());
}

Test(HttpParser, parse_headers_with_invalid_line)
{
    HttpParser parser;
    std::string headers = 
        "Host example.com\n"
        "Content-Type: text/html\n";

    bool res = parser.parse(headers);
    cr_assert(res == true);
    auto &data = parser.get_data();
    cr_assert(data.size() == 1);
    cr_assert(data.at("Content-Type") == "text/html");
}

Test(IniFileParser, parse_file_with_sections)
{
    const char* filename = "test_sections.ini";
    const char* content =
        "# Configuration file\n"
        "[database]\n"
        "host = localhost\n"
        "port = 5432\n"
        "\n"
        "[server]\n"
        "port = 8080\n"
        "root = /var/www/html\n";

    write_temp_ini(filename, content);

    IniFileParser parser;
    bool res = parser.parse(filename);

    std::remove(filename);

    cr_assert(res == true);

    auto& data = parser.get_data();
    cr_assert(data.at("database.host") == "localhost");
    cr_assert(data.at("database.port") == "5432");
    cr_assert(data.at("server.port") == "8080");
    cr_assert(data.at("server.root") == "/var/www/html");
}

Test(IniFileParser, parse_file_with_whitespace_sections)
{
    const char* filename = "test_whitespace_sections.ini";
    const char* content =
        "[  network  ]\n"
        "ip = 192.168.0.1\n"
        "mask = 255.255.255.0\n";

    write_temp_ini(filename, content);

    IniFileParser parser;
    bool res = parser.parse(filename);

    std::remove(filename);

    cr_assert(res == true);

    auto& data = parser.get_data();
    cr_assert(data.at("network.ip") == "192.168.0.1");
    cr_assert(data.at("network.mask") == "255.255.255.0");
}

Test(IniFileParser, parse_file_with_duplicate_keys_in_section)
{
    const char* filename = "test_duplicate_key_section.ini";
    const char* content =
        "[user]\n"
        "name = Alice\n"
        "name = Bob\n";

    write_temp_ini(filename, content);

    IniFileParser parser;
    bool res = parser.parse(filename);

    std::remove(filename);

    cr_assert(res == false);
}

Test(IniFileParser, parse_file_same_key_different_sections)
{
    const char* filename = "test_same_key_diff_sections.ini";
    const char* content = 
        "[client1]\n"
        "timeout = 30\n"
        "[client2]\n"
        "timeout = 60\n";

    write_temp_ini(filename, content);

    IniFileParser parser;
    bool res = parser.parse(filename);

    std::remove(filename);

    cr_assert(res == true, "Parser should allow same key names in different sections");

    auto& data = parser.get_data();
    cr_assert(data.at("client1.timeout") == "30");
    cr_assert(data.at("client2.timeout") == "60");
}

Test(HttpParser, parse_headers_with_multiline_body)
{
    const std::string raw =
        "Content-Type: text/plain\n"
        "\n"
        "Line 1 of body\n"
        "Line 2 of body\n"
        "Line 3 of body\n";

    HttpParser parser;
    bool res = parser.parse(raw);

    cr_assert(res == true, "Parsing headers with multiline body failed");
    cr_assert(parser.get_data().at("Content-Type") == "text/plain");

    std::string expected_body = "Line 1 of body\nLine 2 of body\nLine 3 of body\n";
    cr_assert(parser.get_body() == expected_body);
}

Test(HttpParser, parse_headers_with_empty_body)
{
    const std::string raw =
        "Content-Length: 0\n"
        "\n";

    HttpParser parser;
    bool res = parser.parse(raw);

    cr_assert(res == true, "Parsing headers with empty body failed");
    cr_assert(parser.get_data().at("Content-Length") == "0");
    cr_assert(parser.get_body().empty());
}

Test(HttpParser, parse_headers_with_body_containing_empty_lines)
{
    const std::string raw =
        "Content-Type: text/plain\n"
        "\n"
        "Line 1 of body\n"
        "\n"
        "Line 3 after empty line\n";

    HttpParser parser;
    bool res = parser.parse(raw);

    cr_assert(res == true, "Parsing headers with body containing empty lines failed");
    cr_assert(parser.get_data().at("Content-Type") == "text/plain");

    std::string expected_body = "Line 1 of body\n\nLine 3 after empty line\n";
    cr_assert(parser.get_body() == expected_body);
}

Test(IniFileParser, parse_succeeds_with_required_keys)
{
    IniFileParser parser;
    std::ofstream ofs("test_valid.ini");
    ofs << "[server]\nhost=127.0.0.1\nport=8080\n";
    ofs.close();

    cr_assert(parser.parse("test_valid.ini"));
}

Test(HttpParser, parse_succeeds_with_required_keys)
{
    HttpParser parser;
    std::string headers = "host: localhost\nContent-Type: text/plain\n\nBody data";

    cr_assert(parser.parse(headers));
}

Test(HttpParser, parse_fails_without_required_keys)
{
    HttpParser parser;
    std::string headers = "Content-Type: text/plain\n\nBody data";

    cr_assert_not(parser.parse(headers));
}
