#include "LoadTester.hpp"
#include <curl/curl.h>
#include <thread>
#include <chrono>

LoadTester::LoadTester(const Config &cfg) : cfg(cfg) {}

void LoadTester::run()
{
    curl_global_init(CURL_GLOBAL_ALL);

    double perThreadRps = double(cfg.rps) / cfg.concurrency;
    for (int i = 0; i < cfg.concurrency; ++i)
    {
        workers.emplace_back(cfg, metrics, perThreadRps, stopFlag);
    }
    for (auto &w : workers)
        w.start();

    auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(cfg.duration));
    stopFlag = true;
    for (auto &w : workers)
        w.join();
    auto end = std::chrono::steady_clock::now();

    double elapsed = std::chrono::duration<double>(end - start).count();
    metrics.printSummary(cfg, elapsed);

    curl_global_cleanup();
}
