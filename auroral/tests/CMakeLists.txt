include_directories(../)

add_executable(AURORAL_TEST_EXE parse_test.cpp)
target_link_libraries(AURORAL_TEST_EXE auroral)
add_test(AURORAL_TEST AURORAL_TEST_EXE)