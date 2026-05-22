find_package(GTest REQUIRED)

add_executable(mylib_test mylib_test.cpp)
target_link_libraries(mylib_test PRIVATE mylib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mylib_test)
