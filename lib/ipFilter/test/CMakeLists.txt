add_executable(test_IpFilterLib src/test_ipFilter.cpp)

set_target_properties(test_IpFilterLib PROPERTIES
        CXX_STANDARD_REQUIRED ON
)
target_link_libraries(test_IpFilterLib
        ipFilterLib
        gtest gmock gtest_main
)

add_test(test_IpFilterLib test_IpFilterLib)