cmake_minimum_required(VERSION 3.12)
project(cload LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

include_directories(include)

add_executable(cpp_load_runner
    src/main.cpp
    src/Metrics.cpp
    src/Worker.cpp
    src/LoadTester.cpp
)

target_link_libraries(cpp_load_runner CURL::libcurl)
