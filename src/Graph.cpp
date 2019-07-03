#include "Graph.hpp"

namespace eaframework {


bool Graph::addEdge(int start, int end, int weight) {

}
bool Graph::addEdge(int start, int end) {

}
const Edgelist Graph::getOutEdges(int node) {

}
const Edgelist Graph::getInEdges(int node) {
    
}

std::shared_ptr<Graph> read_graph(std::string filename) {
    auto graph = std::make_shared<Graph>();

    return graph;
}

}
