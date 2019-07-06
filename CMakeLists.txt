cmake_minimum_required(VERSION 3.1...3.14)

project(EAFramework
            VERSION 0.1
            DESCRIPTION "(1+1)EA Mutation Operator testing framework"
            LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(test)
