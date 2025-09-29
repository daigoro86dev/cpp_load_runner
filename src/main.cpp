#include "LoadTester.hpp"
#include "Config.hpp"
#include <getopt.h>
#include <iostream>

void usage(const char *prog)
{
    std::cout << "Usage: " << prog << " --url URL [--concurrency N] [--rps R] [--duration S] [--timeout-ms T]\n";
}

int main(int argc, char **argv)
{
    Config cfg;
    static struct option long_options[] = {
        {"url", required_argument, 0, 'u'},
        {"concurrency", required_argument, 0, 'c'},
        {"rps", required_argument, 0, 'r'},
        {"duration", required_argument, 0, 'd'},
        {"timeout-ms", required_argument, 0, 't'},
        {"method", required_argument, 0, 'm'},
        {"data", required_argument, 0, 'b'},
        {"header", required_argument, 0, 'H'},
        {0, 0, 0, 0}};
    int opt, idx = 0;
    while ((opt = getopt_long(argc, argv, "u:c:r:d:t:", long_options, &idx)) != -1)
    {
        switch (opt)
        {
        case 'u':
            cfg.url = optarg;
            break;
        case 'c':
            cfg.concurrency = std::max(1, atoi(optarg));
            break;
        case 'r':
            cfg.rps = std::max(1, atoi(optarg));
            break;
        case 'd':
            cfg.duration = std::max(1, atoi(optarg));
            break;
        case 't':
            cfg.timeout_ms = std::max(1L, atol(optarg));
            break;
        case 'm':
            cfg.method = optarg;
            break;
        case 'b':
            cfg.body = optarg;
            break;
        case 'H':
            cfg.headers.push_back(optarg);
            break;

        default:
            usage(argv[0]);
            return 1;
        }
    }
    if (cfg.url.empty())
    {
        usage(argv[0]);
        return 1;
    }

    LoadTester tester(cfg);
    tester.run();
    return 0;
}
