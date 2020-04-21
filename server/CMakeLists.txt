add_executable(server main.cpp
)

target_link_libraries(server common)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})