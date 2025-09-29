#pragma once
#include "Config.hpp"
#include "Metrics.hpp"
#include <atomic>
#include <thread>

class Worker
{
public:
    Worker(const Config &cfg, Metrics &metrics, double perThreadRps, std::atomic<bool> &stopFlag);
    void start();
    void join();

private:
    void run();

    const Config &cfg;
    Metrics &metrics;
    double perThreadRps;
    std::atomic<bool> &stopFlag;
    std::thread workerThread;
};
