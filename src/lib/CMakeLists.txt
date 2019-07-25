set(SOURCES
    activity_helpers.cpp
    EA.cpp
    Experiment.cpp
    Graph.cpp
    Individual.cpp
    InformationCollector.cpp
    Instance.cpp
    MutationOperator.cpp
    ObjectiveFunction.cpp
    Runner.cpp
)

set(HEADERS
    activity_helpers.hpp
    EA.hpp
    Experiment.hpp
    Graph.hpp
    helpers.hpp
    Individual.hpp
    InformationCollector.hpp
    Instance.hpp
    MutationOperator.hpp
    ObjectiveFunction.hpp
    Runner.hpp
)

find_package(OpenMP)
if(NOT TARGET OpenMP::OpenMP_CXX)
    find_package(Threads REQUIRED)
    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
    # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)

endif()
set(libraries
    nlohmann_json::nlohmann_json
    OpenMP::OpenMP_CXX
)

add_library(eaframework-lib STATIC ${HEADERS} ${SOURCES})
target_include_directories(eaframework-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(eaframework-lib PUBLIC ${libraries})
