#include "gtest/gtest.h"
#include <Instance.hpp>
#include <Experiment.hpp>
#include <string>
#include <InformationCollector.hpp>
#include <ObjectiveFunction.hpp>
#include <MutationOperator.hpp>

using namespace eaframework;

namespace {

TEST(Experiment, ReadsConfigFile) {
    std::string config_path = "data/config.json";

    auto experiment_config = read_experiment_configuration(config_path);

    EXPECT_EQ(experiment_config.generations, 100);
    EXPECT_EQ(experiment_config.time_limit, 1);
    EXPECT_EQ(experiment_config.information_collector_type, InformationCollectorType::DEFAULT);
    EXPECT_EQ(experiment_config.run_count, 1);
    EXPECT_EQ(experiment_config.objective_function_type, ObjectiveFunctionType::MAXCUT);
    EXPECT_EQ(experiment_config.start_type, StartType::EMPTY);
    ASSERT_EQ(experiment_config.mutation_operator_configs.size(), 1);

    auto& mutation_operator_config = experiment_config.mutation_operator_configs.front();

    EXPECT_EQ(mutation_operator_config.id, 0);
    EXPECT_EQ(mutation_operator_config.type, MutationOperatorType::DEFAULT);
    EXPECT_EQ(mutation_operator_config.parameters.size(), 0);

}

} // namespace
