#include "Graph.hpp"

#include <helpers.hpp>

#include <fstream>
#include <unordered_map>
#include <set>
#include <exception>
#include <sstream>
#include <iostream>
#include <tuple>

namespace {

using namespace eaframework;

enum FileExtension { EDGELIST, MTXREADER, CNFREADER, NXEDGELIST };

static std::unordered_map<std::string, FileExtension> filename_map = {
    {".mtx", MTXREADER},   {".rud", EDGELIST},  {".mc", EDGELIST},
    {".txt", EDGELIST},    {".cnf", CNFREADER}, {".edgelist", NXEDGELIST},
    {".edges", NXEDGELIST}};

class FileReader {
public:
  virtual ~FileReader(){};
  virtual Graph readFile(std::string filename) = 0;
};

class EdgeListReader : public FileReader {
public:
  Graph readFile(std::string filename) override {
    std::ifstream input_file(filename);
    int nodes, edges;
    input_file >> nodes >> edges;
    Graph adj_list{nodes, edges};

    std::set<int> node_names;
    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest >> weight;
      node_names.insert(source);
      node_names.insert(dest);
    }

    std::unordered_map<int, int> node_alias;
    int node_nr = 0;
    for (auto node : node_names) {
      node_alias[node] = node_nr++;
    }

    input_file.clear();
    input_file.seekg(0);

    input_file >> nodes >> edges;

    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest >> weight;
      source = node_alias[source];
      dest = node_alias[dest];
      adj_list.addEdge(source, dest, weight);
    }
    return adj_list;
  }
};

class MTXReader : public FileReader {
public:
  Graph readFile(std::string filename) override {
    std::ifstream input_file(filename);
    // first line is always a comment
    input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int nodes, edges;
    input_file >> nodes >> nodes >> edges;
    ++nodes; // indices start at 1
    Graph adj_list{nodes, edges};

    for (uint i = 0; i < edges; ++i) {
      int source, dest;
      input_file >> source >> dest;
      adj_list.addEdge(source, dest, 1);
    }
    return adj_list;
  }
};

class NXEdgeListReader : public FileReader {
public:
  Graph readFile(std::string filename) override {
    std::ifstream input_file(filename);
    int largest_node = 0;
    int edge_count = 0;
    while (input_file.peek() != EOF) {
      int node;
      if (!(input_file >> node)) {
        ++edge_count;
        input_file.clear();
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
      if (node > largest_node) {
        largest_node = node;
      }
      ++edge_count;
    }

    edge_count /= 2;

    input_file.clear();
    input_file.seekg(0);

    // node indices start at 0, that means we have largest_node + 1 nodes in
    // total
    Graph adj_list{largest_node + 1, edge_count};

    for (uint i = 0; i < edge_count; ++i) {
      int source, dest;
      if (!(input_file >> source >> dest)) {
        input_file.clear();
        input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
      adj_list.addEdge(source, dest, 1);
    }
    return adj_list;
  }
};

class CNFReader : public FileReader {
public:
  Graph readFile(std::string filename) override {
    std::ifstream input_file(filename);
    // first line is a comment
    input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string dummy;
    int nodes, edges;
    // format is
    // p cnf <nodes> <edges>
    input_file >> dummy >> dummy >> nodes >> edges;
    // every vertex (literal) occurs twice, negative ones are negated
    Graph adj_list{nodes * 2, edges};

    for (uint i = 0; i < edges; ++i) {
      int source, dest, weight;
      input_file >> source >> dest;
      // some lines aparrently have only one vertex and a zero - ignore those
      if (dest == 0) {
        continue;
      }
      input_file >> weight;
      // map negative vertices to positive ones
      if (source < 0) {
        source = source * -1 + nodes;
      }
      if (dest < 0) {
        dest = dest * -1 + nodes;
      }
      adj_list.addEdge(source, dest, weight);
    }
    return adj_list;
  }
};

Graph read_graph_impl(std::string basename, std::string extension) {
    FileReader *reader;
    if(filename_map.count(extension) == 0) {
        throw std::runtime_error("Unkown file extension " + extension + " of file " + basename + extension);
    }
    switch (filename_map.at(extension)) {
        case FileExtension::EDGELIST:
            reader = new EdgeListReader();
            break;
        case FileExtension::MTXREADER:
            reader = new MTXReader();
            break;
        case FileExtension::CNFREADER:
            reader = new CNFReader();
            break;
        case FileExtension::NXEDGELIST:
            reader = new NXEdgeListReader();
            break;
    }

    if(!file_exists(basename + extension)) {
      throw std::invalid_argument("File " + basename + extension + " does not exist or is not readable");
    }
    Graph adj_list = reader->readFile(basename + extension);
    delete reader;

    return adj_list;
}

}


namespace eaframework {

bool Graph::addEdge(int start, int end, int weight) {
  if(edgeExists(start, end)) {
    return false;
  }
  edges.push_back({start, end, weight});
  Edge* e_ptr = &(edges.back());
  out_edges[start].push_back(e_ptr);
  in_edges[end].push_back(e_ptr);
  return true;
}

bool Graph::addEdge(int start, int end) {
  return addEdge(start, end, 1);
}
bool Graph::updateEdge(int start, int end, int weight) {
  if(!edgeExists(start, end)) {
    std::stringstream msg;
    msg << "Cannot update edge, it does not exist: " << start << "-" << end << std::endl;
    throw std::invalid_argument(msg.str());
  }

  for(auto edge : out_edges[start]) {
    auto& e_ref = *edge;
    if(e_ref.end == end) {
      e_ref.weight = weight;
      return true;
    }
  }
  return false;
}

bool Graph::edgeExists(int start, int end) {
  for(auto edge : out_edges[start]) {
    auto& e_ref = *edge;
    if(e_ref.end == end) {
      return true;
    }
  }
  return false;
}

const Edgepointers& Graph::getOutEdges(int node) const {
  return out_edges[node];
}
const Edgepointers& Graph::getInEdges(int node) const {
  return in_edges[node];
}

const Edgelist& Graph::getEdges() const {
  return edges;
}

std::shared_ptr<Graph> read_graph(std::string basename, std::string extension) {
    auto graph = ::read_graph_impl(basename, extension);
    auto graph_ptr = std::make_shared<Graph>(std::move(graph));

    return graph_ptr;
}

std::shared_ptr<Graph> read_graph(std::string filename) {
  std::string basename, extension;
  std::tie(basename, extension) = split_filename(filename);
  return read_graph(basename, extension);
}

int Graph::node_count() const {
  return in_edges.size();
}

}
