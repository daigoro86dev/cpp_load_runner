#pragma once
#include <string>

struct Config
{
    std::string url;
    int concurrency = 4;
    int rps = 100;
    int duration = 10; // seconds
    long timeout_ms = 2000;
};
