set(CXXOPTS_BUILD_EXAMPLES OFF)
set(CXXOPTS_BUILD_TESTS OFF)
add_subdirectory(cxxopts)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)

set(INSTALL_GTEST OFF)
add_subdirectory(googletest)
