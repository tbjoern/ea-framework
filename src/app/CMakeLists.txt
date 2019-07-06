set(libraries
    eaframework-lib
    cxxopts
)

add_executable(eaframework main.cpp)
target_link_libraries(eaframework PRIVATE ${libraries})
