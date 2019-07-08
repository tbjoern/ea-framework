#include "gtest/gtest.h"
#include <ObjectiveFunction.hpp>
#include <Instance.hpp>
#include <Individual.hpp>
#include <Graph.hpp>

using namespace eaframework;

namespace {

class ObjectiveFunctionTest : public ::testing::Test {
protected:
    Instance instance;

    ObjectiveFunctionTest()  {
        Graph g{6,5};

        g.addEdge(0,1);
        g.addEdge(0,2);
        g.addEdge(3,0);
        g.addEdge(3,4);
        g.addEdge(0,5);

        Individual start_assignment;
        start_assignment.bit_vector = std::vector<unsigned short>{BIT_ONE, BIT_ZERO, BIT_ZERO, BIT_ZERO, BIT_ZERO, BIT_ONE};

        instance = Instance{std::make_shared<Graph>(std::move(g)), std::make_shared<Individual>(std::move(start_assignment))};
    }
};

TEST_F(ObjectiveFunctionTest, TracksCallCount) {
    auto test = build_objective_function(ObjectiveFunctionType::TEST, instance);

    test->evaluate(*(instance.start_assignment));
    test->evaluate(*(instance.start_assignment));
    test->evaluate(*(instance.start_assignment));

    ASSERT_EQ(test->callCount(), 3);
}

TEST_F(ObjectiveFunctionTest, MaxcutEvaluatesCorrectly) {
    auto maxcut = build_objective_function(ObjectiveFunctionType::MAXCUT, instance);

    double cut_size = maxcut->evaluate(*(instance.start_assignment));

    ASSERT_EQ(cut_size, 3);
}

TEST_F(ObjectiveFunctionTest, MaxdicutEvaluatesCorrectly) {
    auto maxdicut = build_objective_function(ObjectiveFunctionType::MAXDICUT, instance);

    double cut_size = maxdicut->evaluate(*(instance.start_assignment));

    ASSERT_EQ(cut_size, 2);
}

}
