cmake_minimum_required(VERSION 3.1...3.14)

project(EAFramework
            VERSION 0.1
            LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(test)
