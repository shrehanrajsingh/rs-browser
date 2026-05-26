include_directories(../)

add_executable (EQUINOX_TEST_EXE parse_test.cpp)
target_link_libraries(EQUINOX_TEST_EXE equinox)
add_test (EQUINOX_TEST EQUINOX_TEST_EXE)