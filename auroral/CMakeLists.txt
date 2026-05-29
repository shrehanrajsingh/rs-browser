cmake_minimum_required(VERSION 3.10.0)
project(auroral VERSION 0.1.0 LANGUAGES C CXX)

enable_testing()

add_subdirectory(tests)

add_library(
    auroral
    header.hpp
    auroral.hpp auroral.cpp
)

target_link_libraries(auroral PRIVATE mimalloc)
