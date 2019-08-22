#include <string>
#include <iostream>
#include <cxxopts.hpp>
#include <Graph.hpp>

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

int main(int argc, char* argv[]) {
    // parse command line arguments with cxxopt

    cxxopts::Options options("Graph Metadata", "Reads a graph and prints metainformation");

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

    auto graph = eaframework::read_graph(instance_name);

    std::cout << graph->node_count() << " " << graph->getEdges().size() << " " << graph->symmetric() << std::endl;


}
