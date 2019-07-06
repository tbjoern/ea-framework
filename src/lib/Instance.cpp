#include "Instance.hpp"

#include <Graph.hpp>
#include <Individual.hpp>
#include <fstream>

namespace eaframework {

std::shared_ptr<Individual> read_start_assignment(std::string filename) {
    std::ifstream input_file(filename);

    int node_count, node;
    input_file >> node_count;

    auto individual = std::make_shared<Individual>();
    individual->bit_vector.resize(node_count);

    while(input_file.peek() != EOF) {
        input_file >> node;
        individual->bit_vector[node] = BIT_ONE;
    }
    
    return individual;
}

Instance read_instance(std::string instance_name) {
    auto graph = read_graph(instance_name);
    auto start_assignment = read_start_assignment(instance_name + ".assignment");

    return Instance{graph, start_assignment};
}

}
