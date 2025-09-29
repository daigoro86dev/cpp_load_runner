#include "Worker.hpp"
#include <curl/curl.h>
#include <chrono>
#include <thread>

using namespace std::chrono;

static size_t discard_cb(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

Worker::Worker(const Config &cfg, Metrics &metrics, double perThreadRps, std::atomic<bool> &stopFlag)
    : cfg(cfg), metrics(metrics), perThreadRps(perThreadRps), stopFlag(stopFlag) {}

void Worker::start()
{
    workerThread = std::thread(&Worker::run, this);
}

void Worker::join()
{
    if (workerThread.joinable())
        workerThread.join();
}

void Worker::run()
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return;

    double interval = perThreadRps > 0 ? 1.0 / perThreadRps : 0;
    auto nextAllowed = steady_clock::now();

    while (!stopFlag)
    {
        if (interval > 0)
        {
            std::this_thread::sleep_until(nextAllowed);
            nextAllowed += duration_cast<steady_clock::duration>(duration<double>(interval));
        }

        // Reset curl handle for fresh options each request
        curl_easy_reset(curl);

        // Common options
        curl_easy_setopt(curl, CURLOPT_URL, cfg.url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_cb);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, cfg.timeout_ms);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

        // HTTP method & body
        if (cfg.method == "POST")
        {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cfg.body.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, cfg.body.size());
        }
        else if (cfg.method == "PUT" || cfg.method == "PATCH" || cfg.method == "DELETE")
        {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, cfg.method.c_str());
            if (!cfg.body.empty())
            {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cfg.body.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, cfg.body.size());
            }
        }
        // GET is default

        // Headers
        struct curl_slist *header_list = nullptr;
        for (auto &h : cfg.headers)
        {
            header_list = curl_slist_append(header_list, h.c_str());
        }
        if (header_list)
        {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        }

        // Perform
        auto t0 = steady_clock::now();
        CURLcode res = curl_easy_perform(curl);
        auto t1 = steady_clock::now();
        long long latencyUs = duration_cast<microseconds>(t1 - t0).count();

        if (res == CURLE_OK)
        {
            long code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
            if (code >= 200 && code < 400)
                metrics.recordSuccess(latencyUs);
            else
                metrics.recordFailure(latencyUs);
        }
        else
        {
            metrics.recordFailure(latencyUs);
        }

        // Cleanup per-request allocations
        if (header_list)
            curl_slist_free_all(header_list);
    }

    curl_easy_cleanup(curl);
}
