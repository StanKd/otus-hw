add_library(allocatorLib STATIC inc/allocator_lib.hpp)
set_target_properties(allocatorLib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(allocatorLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_subdirectory(test)