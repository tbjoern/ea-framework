#include <string>
#include <iostream>
#include <cxxopts.hpp>
#include <Runner.hpp>

bool require(cxxopts::ParseResult& result, std::string param) {
    if(result.count(param) == 0) {
        std::cout << "Missing parameter: " << param << std::endl;
        return false;   
    }
    return true;
}

bool validate_args(cxxopts::ParseResult& result) {
    if(result["help"].as<bool>()) {
        return false;
    }
    bool valid = true;
    valid &= require(result, "config");
    valid &= require(result, "instance");
    if(!valid) {
        std::cout << std::endl;
    }
    return valid;
}

int main(int argc, char* argv[]) {
    // parse command line arguments with cxxopt

    cxxopts::Options options("EAFramework", "Runs (1+1) Mutation Operators on graphs");

    options.add_options()
        ("h, help", "Display this information")
        ("c,config", "Experiment configuration - REQUIRED", cxxopts::value<std::string>())
        ("i,instance", "Path to graph instance - REQUIRED", cxxopts::value<std::string>())
        ;
    
    auto result = options.parse(argc, argv);

    if(!validate_args(result)) {
        std::cerr << options.help() << std::endl;
        return 1;
    }

    std::string experiment_config_path = result["config"].as<std::string>();
    std::string instance_name = result["instance"].as<std::string>();

    eaframework::execute_runs(experiment_config_path, instance_name);
}
