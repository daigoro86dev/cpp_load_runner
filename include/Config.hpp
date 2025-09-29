// include/Config.hpp
#pragma once
#include <string>
#include <vector>

struct Config
{
    std::string url;
    std::string method = "GET";       // GET, POST, PUT, PATCH, DELETE
    std::string body;                 // request body (for POST/PUT/PATCH)
    std::vector<std::string> headers; // custom headers

    int concurrency = 4;
    int rps = 100;
    int duration = 10; // seconds
    long timeout_ms = 2000;
};
