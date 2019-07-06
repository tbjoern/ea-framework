#include "gtest/gtest.h"
#include <Graph.hpp>

using namespace eaframework;

namespace {

TEST(GraphTest, AddEdge) {
    Graph g{2};
    auto success = g.addEdge(0, 1, 2);

    ASSERT_TRUE(success);
    ASSERT_TRUE(g.edgeExists(0,1));
    EXPECT_EQ(g.getOutEdges(0).size(), 1);
    EXPECT_EQ(g.getInEdges(1).size(), 1);
    EXPECT_EQ(g.getOutEdges(1).size(), 0);
    EXPECT_EQ(g.getInEdges(0).size(), 0);
    EXPECT_EQ(g.getOutEdges(0).front().weight, 2);
}

TEST(GraphTest, UpdateEdge) {
    Graph g{2};
    g.addEdge(0,1,1);
    auto success = g.updateEdge(0,1,10);

    ASSERT_TRUE(success);
    EXPECT_EQ(g.getOutEdges(0).front().weight, 10);
    EXPECT_EQ(g.getInEdges(1).front().weight, 10);
}

TEST(GraphTest, ReadsGraphFromFile) {
    auto graph = read_graph("data/test.edgelist");

    ASSERT_EQ(graph->node_count(), 4);
    EXPECT_TRUE(graph->edgeExists(0,1));
    EXPECT_TRUE(graph->edgeExists(0,2));
    EXPECT_TRUE(graph->edgeExists(0,3));
    EXPECT_TRUE(graph->edgeExists(1,2));
    EXPECT_EQ(graph->getOutEdges(0).front().weight, 1);
    EXPECT_EQ(graph->getOutEdges(1).front().weight, 3);
}

}
