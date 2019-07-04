#include "Instance.hpp"

#include <Graph.hpp>
#include <Individual.hpp>

namespace eaframework {

std::shared_ptr<Individual> read_start_assignment(std::string filename) {
    auto individual = std::make_shared<Individual>();
    
    return individual;
}

Instance read_instance(std::string instance_name) {
    std::string extension = ".mtx";
    auto graph = read_graph(instance_name + extension);
    auto start_assignment = read_start_assignment(instance_name + ".assignment");

    return Instance{graph, start_assignment};
}

}
