find_package(OpenMP)

set(SOURCES
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

set(libraries
    nlohmann_json::nlohmann_json
)

if(OpenMP_CXX_FOUND)
    set(libraries ${libraries} OpenMP::OpenMP_CXX)
endif(OpenMP_CXX_FOUND)

add_library(eaframework-lib STATIC ${HEADERS} ${SOURCES})
target_include_directories(eaframework-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(eaframework-lib PRIVATE ${libraries})
