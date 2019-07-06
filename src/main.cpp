#include <string>
#include <cxxopts.hpp>
#include <nlohmann/json.hpp>
#include <Runner.hpp>

int main(int argc, char* argv[]) {
    // parse command line arguments with cxxopts

    std::string experiment_config_path = "config.json";
    std::string instance_name = "test.edgelist";

    eaframework::execute_runs(experiment_config_path, instance_name);
}
