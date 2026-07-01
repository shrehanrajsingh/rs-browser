cmake_minimum_required(VERSION 3.10.0)
project(solstice VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(tests)

add_library(
    solstice
    header.hpp
    types.hpp
    expr.hpp expr.cpp
    stmt.hpp stmt.cpp
    const.hpp const.cpp
    ast.hpp ast.cpp
    vm.hpp vm.cpp
    codegen.hpp codegen.cpp
    obj.hpp obj.cpp
    frame.hpp frame.cpp
    solstice.hpp solstice.cpp
)

target_link_libraries(solstice PRIVATE mimalloc)