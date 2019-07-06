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
    std::vector<Edgelist> in_edges;
    std::vector<Edgelist> out_edges;

public:
    Graph() {}
    Graph(int node_count) : in_edges(node_count), out_edges(node_count) {}

    bool addEdge(int start, int end, int weight);
    bool addEdge(int start, int end);
    bool updateEdge(int start, int end, int weight);
    bool edgeExists(int start, int end);
    const Edgelist getOutEdges(int node);
    const Edgelist getInEdges(int node);
    int node_count() const;
};

std::shared_ptr<Graph> read_graph(std::string filename);

}
