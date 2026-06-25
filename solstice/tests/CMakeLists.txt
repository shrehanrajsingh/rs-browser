include_directories(../)

add_executable (SOLSTICE_TEST_EXE fulljs_test.cpp)
target_link_libraries(SOLSTICE_TEST_EXE solstice)
add_test (SOLSTICE_TEST SOLSTICE_TEST_EXE)