add_executable(storage_tests
    test_page.cpp
)

target_link_libraries(storage_tests
    storage_engine
    gtest
    gtest_main
)

add_test(NAME storage_tests COMMAND storage_tests)
