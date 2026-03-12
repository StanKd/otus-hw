cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

option(WITH_GTEST "Whether to build Google test" OFF)

configure_file(version.h.in version.h)

add_executable(ip_filter ip_filter.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

add_definitions(
#        -DPRINT_TIME
)

if(WIN32)
    target_link_libraries(ip_filter ws2_32)
endif()

target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if(WITH_GTEST)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.17.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable (testVersion test_version.cpp)

    target_link_libraries(
            testVersion
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(testVersion)

    set_target_properties(testVersion PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(testVersion
        helloworld
    )
endif()

if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    if(WITH_GTEST)
        target_compile_options(testVersion PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_GTEST)
        target_compile_options(testVersion PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_GTEST)
    enable_testing()
    add_test(testVersion testVersion)
endif()
