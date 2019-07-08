set(libraries
    eaframework-lib
    cxxopts
)

add_executable(eaframework eaframework.cpp)
target_link_libraries(eaframework PRIVATE ${libraries})
