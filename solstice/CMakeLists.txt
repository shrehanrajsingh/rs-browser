cmake_minimum_required(VERSION 3.10.0)
project(solstice VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(tests)

add_library(
    solstice
    header.hpp
    solstice.hpp solstice.cpp
)

target_link_libraries(solstice PRIVATE mimalloc)