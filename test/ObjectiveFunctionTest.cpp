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
        std::shared_ptr<Graph> g = std::make_shared<Graph>(6,5);

        g->addEdge(0,1);
        g->addEdge(0,2);
        g->addEdge(3,0);
        g->addEdge(3,4);
        g->addEdge(0,5);

        Individual start_assignment;
        start_assignment.bit_vector = std::vector<Bit>{BIT_ONE, BIT_ZERO, BIT_ZERO, BIT_ZERO, BIT_ZERO, BIT_ONE};

        instance = Instance{g, std::make_shared<Individual>(std::move(start_assignment))};
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

TEST_F(ObjectiveFunctionTest, MaxcutRespectsBITNONE) {
    auto maxcut = build_objective_function(ObjectiveFunctionType::MAXCUT, instance);
    Individual test = *(instance.start_assignment);
    for(auto& bit : test.bit_vector) {
        bit = BIT_NONE;
    }
    test.bit_vector[0] = BIT_ONE;
    test.bit_vector[1] = BIT_ZERO;

    double cut_size = maxcut->evaluate(test);

    ASSERT_EQ(cut_size, 1);
}

TEST_F(ObjectiveFunctionTest, MaxdicutRespectsBITNONE) {
    auto maxcut = build_objective_function(ObjectiveFunctionType::MAXCUT, instance);
    Individual test = *(instance.start_assignment);
    for(auto& bit : test.bit_vector) {
        bit = BIT_NONE;
    }
    test.bit_vector[0] = BIT_ONE;
    test.bit_vector[1] = BIT_ZERO;

    double cut_size = maxcut->evaluate(test);

    ASSERT_EQ(cut_size, 1);
}

}
