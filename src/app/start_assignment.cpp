#include <string>
#include <iostream>
#include <cxxopts.hpp>
#include <Graph.hpp>
#include <vector>
#include <random>
#include <Individual.hpp>

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
    valid &= require(result, "instance");
    if(!valid) {
        std::cout << std::endl;
    }
    return valid;
}

using namespace eaframework;

int main(int argc, char* argv[]) {
    // parse command line arguments with cxxopt

    cxxopts::Options options("Start assignment", "Creates random bitvectors for graphs");

    options.add_options()
        ("h, help", "Display this information")
        ("i,instance", "Path to graph instance - REQUIRED", cxxopts::value<std::string>())
        ;
    
    auto result = options.parse(argc, argv);

    if(!validate_args(result)) {
        std::cerr << options.help() << std::endl;
        return 1;
    }

    std::string instance_name = result["instance"].as<std::string>();

    auto graph = read_graph(instance_name);
    Individual start_assignment;
    start_assignment.bit_vector.resize(graph->node_count());

    std::mt19937 engine(std::random_device{}());
    std::bernoulli_distribution dist(0.5);

    for(auto& bit : start_assignment.bit_vector) {
        if(dist(engine)) {
            bit = BIT_ONE;
        }
    }

    std::cout << start_assignment;
}
