cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0048 NEW)
project(pichat VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)