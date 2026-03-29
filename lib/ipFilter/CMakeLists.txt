add_library(ipFilterLib STATIC src/ipFilter_lib.cpp)

target_include_directories(ipFilterLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_subdirectory(test)