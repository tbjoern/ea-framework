#include "gtest/gtest.h"
#include <activity_helpers.hpp>
#include <Instance.hpp>
#include <ObjectiveFunction.hpp>
#include <Graph.hpp>

using namespace eaframework;

namespace {

class ActivityHelperTest : public ::testing::Test {
protected:
    Instance instance;
    std::shared_ptr<ObjectiveFunction> maxdicut;
    activity::Parameters params;

    ActivityHelperTest()  {
        Graph g{6,5};

        g.addEdge(0,1);
        g.addEdge(0,2);
        g.addEdge(3,0);
        g.addEdge(3,4);
        g.addEdge(0,5);

        Individual start_assignment;
        start_assignment.bit_vector = std::vector<Bit>(6, BIT_ZERO);

        instance = Instance{std::make_shared<Graph>(std::move(g)), std::make_shared<Individual>(std::move(start_assignment))};
        maxdicut = build_objective_function(ObjectiveFunctionType::MAXDICUT, instance);

        params.start = 1;
    }
};

TEST_F(ActivityHelperTest, InitsActivity) {
    Individual test = Individual(*(instance.start_assignment));

    ASSERT_EQ(test.data_vectors.count("activity"), 0);
    activity::init(params, test);
    ASSERT_EQ(test.data_vectors.count("activity"), 1);
    EXPECT_EQ(test.data_vectors["activity"].size(), 6);
    for(auto v : test.data_vectors["activity"]) {
        EXPECT_EQ(v, params.start);
    }
}

TEST_F(ActivityHelperTest, CalculatesActivityValues) {
    Individual test = Individual(*(instance.start_assignment));

    auto zero_individual = Individual();
    zero_individual.bit_vector = std::vector<Bit>(test.bit_vector.size(), BIT_NONE);

    for(int node = 0; node < 6; ++node) {
        for(int node2 = 0; node2 < 6; ++node2) {
            if(instance.graph->edgeExists(node, node2)) {
                EXPECT_EQ(activity::activity_value(zero_individual, *maxdicut, node, node2), 1);
            }
            else {
                EXPECT_EQ(activity::activity_value(zero_individual, *maxdicut, node, node2), 0);
            }
        }
    }
}

TEST_F(ActivityHelperTest, UpdatesActivityValues) {
    Individual test = Individual(*(instance.start_assignment));
    auto zero_individual = Individual();
    zero_individual.bit_vector = std::vector<Bit>(test.bit_vector.size(), BIT_NONE);

    activity::init(params, test);
    test.bit_vector[0] = BIT_ONE;
    test.bit_vector[1] = BIT_ONE;

    activity::update(params, test, {0,1}, zero_individual, *maxdicut);

    auto& activity = test.data_vectors["activity"];

    EXPECT_EQ(activity[0], params.start);
    EXPECT_EQ(activity[1], params.start);
    EXPECT_EQ(activity[2], params.start - params.dec);
    EXPECT_EQ(activity[3], params.start);
    EXPECT_EQ(activity[4], params.start);
    EXPECT_EQ(activity[5], params.start - params.dec);

    test.bit_vector[3] = BIT_ONE;
    activity::update(params, test, {3}, zero_individual, *maxdicut);
    EXPECT_EQ(activity[0], params.start + params.inc);
    EXPECT_EQ(activity[4], params.start - params.dec);

    test.bit_vector[3] = BIT_ZERO;
    test.bit_vector[0] = BIT_ZERO;
    activity::update(params, test, {0}, zero_individual, *maxdicut);
    EXPECT_EQ(activity[1], params.start); // stays the same
    EXPECT_EQ(activity[2], params.start - params.dec); // stays the same
    EXPECT_EQ(activity[3], params.start + params.inc);
}

}
