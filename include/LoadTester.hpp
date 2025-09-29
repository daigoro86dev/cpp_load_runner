#pragma once
#include "Config.hpp"
#include "Metrics.hpp"
#include "Worker.hpp"
#include <vector>
#include <atomic>

class LoadTester
{
public:
    LoadTester(const Config &cfg);
    void run();

private:
    const Config &cfg;
    Metrics metrics;
    std::atomic<bool> stopFlag{false};
    std::vector<Worker> workers;
};
