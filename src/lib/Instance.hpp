#pragma once
#include <memory>
#include <vector>
#include <string>

namespace eaframework {

class Graph;
enum class StartType {
    EMPTY = 0,
    RANDOM = 1,
    FULL = 2,
};

struct Individual;

struct Instance {
    std::shared_ptr<const Graph> graph;
    std::shared_ptr<const Individual> start_assignment;
};

std::shared_ptr<Individual> read_start_assignment(std::string filename);
Instance read_instance(std::string instance_name, StartType start_type);

}
