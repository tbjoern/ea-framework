set(libraries
    eaframework-lib
    cxxopts
)

add_executable(eaframework eaframework.cpp)
target_link_libraries(eaframework PRIVATE ${libraries})

add_executable(start_assignment start_assignment.cpp)
target_link_libraries(start_assignment PRIVATE ${libraries})
