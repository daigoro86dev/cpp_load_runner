#pragma once
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>

class Metrics
{
public:
    void recordSuccess(long long latencyUs);
    void recordFailure(long long latencyUs);

    void printSummary(const class Config &cfg, double elapsedSeconds) const;

private:
    std::atomic<uint64_t> totalSent{0};
    std::atomic<uint64_t> totalSuccess{0};
    std::atomic<uint64_t> totalFail{0};

    mutable std::mutex latMutex;
    std::vector<long long> latenciesUs;
};
