cmake_minimum_required(VERSION 3.10.0)
project(auroral VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(tests)

add_library(
    auroral
    header.hpp
    auroral.hpp auroral.cpp
    s_base.hpp
    s_display.hpp
    s_font.hpp
    s_height.hpp
    s_padding.hpp
    s_width.hpp
    parser.hpp parser.cpp
    css_node.hpp css_node.cpp
)

target_link_libraries(auroral PRIVATE mimalloc)
