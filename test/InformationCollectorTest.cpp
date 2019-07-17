#include "gtest/gtest.h"

#include <InformationCollector.hpp>
#include <EA.hpp>
#include <Instance.hpp>
#include <ObjectiveFunction.hpp>
#include <MutationOperator.hpp>
#include <Experiment.hpp>
#include <Individual.hpp>
#include <memory>
#include <sstream>

using namespace eaframework;

namespace {

class InformationCollectorTest : public ::testing::Test {
public:
    Instance instance;
    std::shared_ptr<ObjectiveFunction> maxcut;
    std::shared_ptr<MutationOperator> mutation_operator;
    std::unique_ptr<EA> ea;

    InformationCollectorTest() {
        instance = read_instance("data/InformationCollectorTest/test.txt", true);
        maxcut = build_objective_function(ObjectiveFunctionType::MAXCUT, instance);
        auto config = MutationOperatorConfig{};
        config.id = 0;
        config.type = MutationOperatorType::DEFAULT;
        mutation_operator = build_mutation_operator(config);
        ea = std::make_unique<EA>(maxcut, mutation_operator);
    }
};

TEST_F(InformationCollectorTest, RecordsIterationData) {
    auto information_collector = build_information_collector(InformationCollectorType::ITERATIONDATA);

    ea->make_initial_individual(instance);
    information_collector->generation_snapshot(0, 0, 0, *ea);
    
    std::stringstream s;
    information_collector->output_to_stream(s);

    EXPECT_GT(s.str().size(), 0);
}

TEST_F(InformationCollectorTest, RecordsSuccessiveIterationData) {
    auto information_collector = build_information_collector(InformationCollectorType::ITERATIONDATA);

    ea->make_initial_individual(instance);
    information_collector->generation_snapshot(0, 0, 0,*ea);
    ea->next_generation();
    information_collector->generation_snapshot(0, 0, 1,*ea);
    
    std::stringstream s;
    information_collector->output_to_stream(s);

    EXPECT_GT(s.str().size(), 0);
}

} // namespace
