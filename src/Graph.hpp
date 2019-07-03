#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace eaframework {

struct Edge {
    int end;
    int weight;
};
typedef std::vector<Edge> Edgelist;

class Graph {
    Edgelist in_edges;
    Edgelist out_edges;

public:
    bool addEdge(int start, int end, int weight);
    bool addEdge(int start, int end);
    const Edgelist getOutEdges(int node);
    const Edgelist getInEdges(int node);
};

std::shared_ptr<Graph> read_graph(std::string filename);

}
