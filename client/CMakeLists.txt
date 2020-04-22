find_package (Threads)

add_executable(client main.cpp
                      Client.cpp
)

target_link_libraries(client common ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})