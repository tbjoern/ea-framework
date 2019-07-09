#include "gtest/gtest.h"
#include <EA.hpp>
#include <Instance.hpp>
#include <ObjectiveFunction.hpp>
#include <MutationOperator.hpp>
#include <Experiment.hpp>
#include <memory>

using namespace eaframework;

namespace {

class MutationOperatorTest : public ::testing::Test {
public:
    Instance instance;
    std::shared_ptr<ObjectiveFunction> maxcut;
    MutationOperatorConfig config;

    MutationOperatorTest() {
        instance = read_instance("data/MutationOperatorTest/test.txt");
        maxcut = build_objective_function(ObjectiveFunctionType::MAXCUT, instance);
        config.id = 0;
    }
};

TEST_F(MutationOperatorTest, Unif) {
    config.type = MutationOperatorType::UNIF;

    auto mutation_operator = build_mutation_operator(config);
    
    auto ea = EA(maxcut, mutation_operator);

    ea.make_initial_individual(instance);
    ea.next_generation();
}

TEST_F(MutationOperatorTest, FMUT) {
    config.type = MutationOperatorType::FMUT;
    config.parameters.push_back({"power_law_beta", 2.5});

    auto mutation_operator = build_mutation_operator(config);
    
    auto ea = EA(maxcut, mutation_operator);

    ea.make_initial_individual(instance);
    ea.next_generation();
}

TEST_F(MutationOperatorTest, PMUT) {
    config.type = MutationOperatorType::PMUT;
    config.parameters.push_back({"power_law_beta", 2.5});

    auto mutation_operator = build_mutation_operator(config);
    
    auto ea = EA(maxcut, mutation_operator);

    ea.make_initial_individual(instance);
    ea.next_generation();
}

} // namespace
