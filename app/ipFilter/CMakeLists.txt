add_executable(${PROJECT_NAME} src/main.cpp)

add_dependencies(${PROJECT_NAME} ipFilterLib)
target_link_libraries(${PROJECT_NAME} ipFilterLib)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)