cmake_minimum_required(VERSION 3.10.0)
project(rsbrowser VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

include(FetchContent)

# ADD YOUR OWN QT INSTALLATION PATH
list(PREPEND CMAKE_PREFIX_PATH "/Volumes/Seagate/QT2/6.11.1/macos")
include_directories(/Volumes/Seagate/QT2/6.11.1/macos/lib)

set(MI_OVERRIDE ON CACHE BOOL "" FORCE)
set(MI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MI_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(MI_BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(MI_USE_CXX OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    mimalloc
    GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
    GIT_TAG v2.1.7
)

FetchContent_MakeAvailable(mimalloc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Network Sql Xml)

add_subdirectory(equinox)
add_subdirectory(auroral)

add_executable(
    rsbrowser
    w_browser.hpp w_browser.cpp
    v_browser.hpp v_browser.cpp
    header.hpp
    main.hpp main.cpp)

target_link_libraries(rsbrowser PRIVATE 
    mimalloc
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Sql
    Qt6::Xml
    equinox
    auroral)