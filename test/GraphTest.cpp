#include "gtest/gtest.h"
#include <Graph.hpp>

using namespace eaframework;

namespace {

void validate_graph(std::shared_ptr<Graph> graph) {
    ASSERT_EQ(graph->node_count(), 4);

    EXPECT_TRUE(graph->edgeExists(0,1));
    EXPECT_TRUE(graph->edgeExists(1,2));
    EXPECT_TRUE(graph->edgeExists(0,2));
    EXPECT_TRUE(graph->edgeExists(0,3));
}

TEST(Graph, AddEdge) {
    Graph g{2, 1};
    auto success = g.addEdge(0, 1, 2);

    ASSERT_TRUE(success);
    ASSERT_TRUE(g.edgeExists(0,1));
    EXPECT_EQ(g.getOutEdges(0).size(), 1);
    EXPECT_EQ(g.getInEdges(1).size(), 1);
    EXPECT_EQ(g.getOutEdges(1).size(), 0);
    EXPECT_EQ(g.getInEdges(0).size(), 0);
    EXPECT_EQ(g.getOutEdges(0).front()->weight, 2);
}

TEST(Graph, UpdateEdge) {
    Graph g{2, 1};
    g.addEdge(0,1,1);
    auto success = g.updateEdge(0,1,10);

    ASSERT_TRUE(success);
    EXPECT_EQ(g.getOutEdges(0).front()->weight, 10);
    EXPECT_EQ(g.getInEdges(1).front()->weight, 10);
}

TEST(Graph, ReadsEdgelist) {
    auto graph = read_graph("data/test.txt");
    validate_graph(graph);

    EXPECT_EQ(graph->getOutEdges(0).front()->weight, 1);
    EXPECT_EQ(graph->getOutEdges(1).front()->weight, 3);
}

TEST(Graph, ReadsSymmetricMTX) {
    auto graph = read_graph("data/test-symmetric.mtx");
    validate_graph(graph);
    EXPECT_EQ(graph->getEdges().size(), 8);
}

TEST(Graph, ReadsGeneralMTX) {
    auto graph = read_graph("data/test-general.mtx");
    validate_graph(graph);
    EXPECT_EQ(graph->getEdges().size(), 4);
}

TEST(Graph, ReadsNXEdgelist) {
    auto graph = read_graph("data/test.edgelist");
    validate_graph(graph);
}

}
