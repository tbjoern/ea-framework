#include <memory>
#include <vector>
#include <string>

class Graph;

namespace eaframework {

typedef std::vector<unsigned short> Individual;

struct Instance {
    std::shared_ptr<const Graph> graph;
    std::shared_ptr<const Individual> start_assignment;
};

std::shared_ptr<Individual> read_start_assignment(std::string filename);
Instance read_instance(std::string instance_name);

}
