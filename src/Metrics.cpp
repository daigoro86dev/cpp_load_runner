#include "Metrics.hpp"
#include "Config.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>

void Metrics::recordSuccess(long long latencyUs)
{
    totalSent++;
    totalSuccess++;
    std::lock_guard<std::mutex> lk(latMutex);
    latenciesUs.push_back(latencyUs);
}

void Metrics::recordFailure(long long latencyUs)
{
    totalSent++;
    totalFail++;
    std::lock_guard<std::mutex> lk(latMutex);
    latenciesUs.push_back(latencyUs);
}

void Metrics::printSummary(const Config &cfg, double elapsedSeconds) const
{
    std::cout << "\n=== Summary ===\n";
    std::cout << "Duration (s): " << elapsedSeconds << "\n";
    std::cout << "Target RPS: " << cfg.rps << "\n";
    std::cout << "Concurrency: " << cfg.concurrency << "\n";
    std::cout << "Total requests: " << totalSent << "\n";
    std::cout << "Successful: " << totalSuccess << "\n";
    std::cout << "Failed: " << totalFail << "\n";
    std::cout << "Achieved RPS: " << totalSent / elapsedSeconds << "\n";

    std::lock_guard<std::mutex> lk(latMutex);
    if (!latenciesUs.empty())
    {
        auto lat = latenciesUs;
        std::sort(lat.begin(), lat.end());
        double sum = std::accumulate(lat.begin(), lat.end(), 0.0);
        auto percentile = [&](double q)
        {
            size_t idx = (size_t)(q * lat.size());
            if (idx >= lat.size())
                idx = lat.size() - 1;
            return lat[idx];
        };
        std::cout << "Latency (ms): min=" << lat.front() / 1000.0
                  << " mean=" << (sum / lat.size()) / 1000.0
                  << " p50=" << percentile(0.50) / 1000.0
                  << " p90=" << percentile(0.90) / 1000.0
                  << " p95=" << percentile(0.95) / 1000.0
                  << " p99=" << percentile(0.99) / 1000.0
                  << " max=" << lat.back() / 1000.0 << "\n";
    }
}
