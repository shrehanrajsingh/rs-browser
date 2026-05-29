cmake_minimum_required(VERSION 3.10.0)
project(equinox VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(tests)

add_library(
    equinox
    header.hpp
    node.hpp node.cpp
    ndata.hpp ndata.cpp
    parser.hpp parser.cpp
    equinox.hpp equinox.cpp
)

target_link_libraries(equinox PRIVATE mimalloc)