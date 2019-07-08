#include "gtest/gtest.h"
#include <EA.hpp>
#include <Instance.hpp>
#include <ObjectiveFunction.hpp>
#include <MutationOperator.hpp>
#include <Experiment.hpp>
#include <Individual.hpp>
#include <memory>

using namespace eaframework;

namespace {

class EATest : public ::testing::Test {
public:
    Instance instance;
    std::shared_ptr<ObjectiveFunction> maxcut;
    std::shared_ptr<MutationOperator> mutation_operator;

    EATest() {
        instance = read_instance("data/EATest/test.txt");
        maxcut = build_objective_function(ObjectiveFunctionType::MAXCUT, instance);
        auto config = MutationOperatorConfig{};
        config.id = 0;
        config.type = MutationOperatorType::DEFAULT;
        mutation_operator = build_mutation_operator(config);
    }
};

TEST_F(EATest, InitializesIndividual) {
    auto ea = EA(maxcut, mutation_operator);

    ea.make_initial_individual(instance);
}

TEST_F(EATest, TestRun) {
    auto ea = EA(maxcut, mutation_operator);

    ea.make_initial_individual(instance);
    ea.next_generation();

    ASSERT_TRUE(ea.generationImproved());

    auto individual = ea.getBestIndividual();
    EXPECT_EQ(individual.bit_vector[0], BIT_ONE);
}

} // namespace
