add_executable(test_allocatorLib src/allocator_test.cpp)

set_target_properties(test_allocatorLib PROPERTIES
        CXX_STANDARD_REQUIRED ON
)
target_link_libraries(test_allocatorLib
        allocatorLib
        gtest gmock gtest_main
)

add_test(test_allocatorLib test_allocatorLib)