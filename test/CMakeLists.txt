set(SOURCES
    ActivityHelperTest.cpp
    main.cpp
    EATest.cpp
    ExperimentTest.cpp
    GraphTest.cpp
    InformationCollectorTest.cpp
    InstanceTest.cpp
    MutationOperatorTest.cpp
    ObjectiveFunctionTest.cpp
)

set(HEADERS
    
)

set(libraries
    gtest
    gtest_main
    eaframework-lib
)

add_executable(tests ${HEADERS} ${SOURCES})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE ${libraries})
