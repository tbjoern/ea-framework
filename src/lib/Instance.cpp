#include "Instance.hpp"

#include <helpers.hpp>

#include <Graph.hpp>
#include <Individual.hpp>
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

    while(input_file.peek() != EOF) {
        input_file >> node;
        if(!input_file.good()){
            break;
        }
        individual->bit_vector[node] = BIT_ONE;
    }
    
    return individual;
}

Instance read_instance(std::string instance_name) {
    std::string basename, extension;
    std::tie(basename, extension) = split_filename(instance_name);

    auto graph = read_graph(basename, extension);
    auto start_assignment = read_start_assignment(basename + ".assignment");

    return Instance{graph, start_assignment};
}

}
