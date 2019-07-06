set(SOURCES
    main.cpp
)

set(HEADERS
    
)

set(libraries
    gtest_main
)

add_executable(tests ${HEADERS} ${SOURCES})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE ${libraries})
