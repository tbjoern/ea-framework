find_package(OpenMP)

set(SOURCES
    EA.cpp
    Experiment.cpp
    Graph.cpp
    InformationCollector.cpp
    Instance.cpp
    MutationOperatorFactory.cpp
    main.cpp
    ObjectiveFunction.cpp
    Runner.cpp
)

set(HEADERS
    EA.hpp
    Experiment.hpp
    Graph.hpp
    InformationCollector.hpp
    Instance.hpp
    MutationOperator.hpp
    MutationOperatorFactory.hpp
    ObjectiveFunction.hpp
    Runner.hpp
)

set(libraries
    cxxopts
    nlohmann_json::nlohmann_json
)

if(OpenMP_CXX_FOUND)
    set(libraries ${libraries} OpenMP::OpenMP_CXX)
endif(OpenMP_CXX_FOUND)

add_executable(eaframework ${HEADERS} ${SOURCES})
target_include_directories(eaframework PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(eaframework PRIVATE ${libraries})
