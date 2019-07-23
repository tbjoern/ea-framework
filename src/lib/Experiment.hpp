#pragma once
#include <string>
#include <vector>
#include <string>
#include <memory>

namespace eaframework {

class Instance;
enum class ObjectiveFunctionType;
enum class InformationCollectorType;
enum class MutationOperatorType;

struct MutationOperatorParameter {
    std::string name;
    double value;
};

struct MutationOperatorConfig {
    MutationOperatorType type;
    uint id;
    std::vector<MutationOperatorParameter> parameters;
};


struct ExperimentConfig {
    int run_count;
    unsigned int time_limit;
    bool use_predefined_start;
    ObjectiveFunctionType objective_function_type;
    InformationCollectorType information_collector_type;
    int generation_count;
    std::shared_ptr<const Instance> instance;
    std::vector<MutationOperatorConfig> mutation_operator_configs;
};

ExperimentConfig read_experiment_configuration(std::string);

}
