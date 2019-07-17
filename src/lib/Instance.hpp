#pragma once
#include <memory>
#include <vector>
#include <string>

namespace eaframework {

class Graph;

struct Individual;

struct Instance {
    std::shared_ptr<const Graph> graph;
    std::shared_ptr<const Individual> start_assignment;
};

std::shared_ptr<Individual> read_start_assignment(std::string filename);
Instance read_instance(std::string instance_name, bool use_start_assignment);

}
