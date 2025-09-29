# cpp_load_runner

A lightweight, extensible HTTP load testing CLI tool written in C++17.\
It allows you to send **dozens to hundreds of HTTP requests per second**
against an endpoint, supporting all common HTTP methods (GET, POST, PUT,
PATCH, DELETE), with configurable concurrency, rate, and duration.

Built with: - **CMake** as the build system - **libcurl** for HTTP
client functionality - **OOP structure** (Config, Metrics, Worker,
LoadTester classes) - Designed to run on Linux (tested on Fedora) and in
Docker

------------------------------------------------------------------------

## 📂 Project Structure

    cpp_load_runner/
    ├── CMakeLists.txt
    ├── include/
    │   ├── Config.hpp
    │   ├── Metrics.hpp
    │   ├── Worker.hpp
    │   └── LoadTester.hpp
    ├── src/
    │   ├── Metrics.cpp
    │   ├── Worker.cpp
    │   ├── LoadTester.cpp
    │   └── main.cpp
    └── Dockerfile

------------------------------------------------------------------------

## ⚙️ Build & Run (native)

### Dependencies

On Fedora (similar on other distros):

``` bash
sudo dnf install cmake gcc-c++ libcurl-devel
```

### Build

``` bash
mkdir build && cd build
cmake ..
make -j
```

### Run

``` bash
./cpp_load_runner --url https://example.com --concurrency 4 --rps 100 --duration 10
```

------------------------------------------------------------------------

## 🖥️ CLI Options

  ----------------------------------------------------------------------------
  Option              Description                                    Default
  ------------------- ---------------------------------------------- ---------
  `--url URL`         Target endpoint (required)                     ---

  `--concurrency N`   Number of worker threads                       4

  `--rps R`           Target aggregate requests per second           100

  `--duration S`      Test duration in seconds                       10

  `--timeout-ms T`    Per-request timeout in milliseconds            2000

  `--method M`        HTTP method: GET, POST, PUT, PATCH, DELETE     GET

  `--data BODY`       Request body (for POST/PUT/PATCH)              empty

  `--header H`        Custom header(s), can be specified multiple    none
                      times                                          
  ----------------------------------------------------------------------------

### Examples

``` bash
# Simple GET
./cpp_load_runner --url https://api.example.com/items

# POST JSON
./cpp_load_runner --url https://api.example.com/items         --method POST         --header "Content-Type: application/json"         --data '{"name":"test"}'

# PUT with JSON body
./cpp_load_runner --url https://api.example.com/items/42         --method PUT         --header "Content-Type: application/json"         --data '{"name":"updated"}'

# DELETE
./cpp_load_runner --url https://api.example.com/items/42 --method DELETE
```

------------------------------------------------------------------------

## 📊 Output

After a run, the CLI prints a summary like:

    === Summary ===
    Duration (s): 10
    Target RPS: 100
    Concurrency: 4
    Total requests: 1000
    Successful: 980
    Failed: 20
    Achieved RPS: 99.8
    Latency (ms): min=12.3 mean=35.7 p50=30.1 p90=55.2 p95=70.4 p99=110.7 max=150.3

------------------------------------------------------------------------

## 🐳 Docker

### Build image

``` bash
docker build -t cpp_load_runner .
```

### Run load test

``` bash
docker run --rm cpp_load_runner   --url https://example.com   --concurrency 8   --rps 200   --duration 15
```

------------------------------------------------------------------------

## 🚨 Notes & Warnings

-   Use only against systems you **own or have explicit permission** to
    test.
-   The tool measures **client-side latency** (network + scheduling
    included).
-   System limits (CPU, NIC, ulimit, TCP settings) can cap achievable
    RPS.
-   For very high loads (\>thousands RPS), async/event-driven I/O may be
    needed.

------------------------------------------------------------------------

## 📌 Roadmap / Possible Enhancements

-   Live per-second stats during test
-   JSON/CSV output for metrics
-   Latency histogram with HDRHistogram
-   Configurable connection reuse and HTTP/2
-   Distributed test coordination

------------------------------------------------------------------------

### License

[MIT](./LICENSE) © 2025 Bruno Maio