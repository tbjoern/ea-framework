#include "Instance.hpp"

#include <helpers.hpp>

#include <Graph.hpp>
#include <Individual.hpp>
#include <ObjectiveFunction.hpp>
#include <fstream>
#include <tuple>

namespace eaframework {

std::shared_ptr<Individual> read_start_assignment(std::string filename) {
    std::ifstream input_file(filename);
    if(!input_file.good()) {
        throw std::invalid_argument("File " + filename + " is not readable");
    }

    int node_count, node;
    input_file >> node_count;

    auto individual = std::make_shared<Individual>();
    individual->bit_vector.resize(node_count);
    for(auto& bit : individual->bit_vector) {
        bit = BIT_ZERO;
    }

    while(input_file.peek() != EOF) {
        input_file >> node;
        if(!input_file.good()){
            break;
        }
        individual->bit_vector[node] = BIT_ONE;
    }
    
    return individual;
}

Instance read_instance(std::string instance_name, StartType start_type, ObjectiveFunctionType obj_func) {
    std::string basename, extension;
    std::tie(basename, extension) = split_filename(instance_name);

    bool directed = true;
    if(obj_func == ObjectiveFunctionType::MAXCUT) {
        directed = false;
    }

    auto graph = read_graph(basename, extension, directed);
    std::shared_ptr<Individual> start_assignment;
    switch(start_type) {
        case StartType::EMPTY:
            start_assignment = std::make_shared<Individual>();
            start_assignment->bit_vector.resize(graph->node_count());
            for(auto& bit : start_assignment->bit_vector) {
                bit = BIT_ZERO;
            }
            break;
        case StartType::RANDOM:
            start_assignment = read_start_assignment(basename + ".assignment");
            break;
        case StartType::FULL:
            start_assignment = std::make_shared<Individual>();
            start_assignment->bit_vector.resize(graph->node_count());
            for(auto& bit : start_assignment->bit_vector) {
                bit = BIT_ONE;
            }
            break;
    }

    return Instance{graph, start_assignment};
}

}
