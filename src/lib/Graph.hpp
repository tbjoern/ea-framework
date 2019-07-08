#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace eaframework {

struct Edge {
    int start;
    int end;
    int weight;
};
typedef std::vector<Edge> Edgelist;
typedef std::vector<Edge*> Edgepointers;

class Graph {
    Edgelist edges;
    std::vector<Edgepointers> in_edges;
    std::vector<Edgepointers> out_edges;

public:
    Graph() {}
    Graph(int node_count, int edge_count) : edges(0), in_edges(node_count), out_edges(node_count) {
        edges.reserve(edge_count);
    }

    bool addEdge(int start, int end, int weight);
    bool addEdge(int start, int end);
    bool updateEdge(int start, int end, int weight);
    bool edgeExists(int start, int end);
    const Edgepointers& getOutEdges(int node) const;
    const Edgepointers& getInEdges(int node) const;
    const Edgelist& getEdges() const;
    int node_count() const;
};

std::shared_ptr<Graph> read_graph(std::string filename);
std::shared_ptr<Graph> read_graph(std::string basename, std::string extension);

}