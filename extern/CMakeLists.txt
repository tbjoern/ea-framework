add_subdirectory(cxxopts)

add_library(inih INTERFACE)
target_include_directories(inih INTERFACE inih)
