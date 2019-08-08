#include "Experiment.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <InformationCollector.hpp>
#include <ObjectiveFunction.hpp>
#include <MutationOperator.hpp>
#include <Instance.hpp>

using json = nlohmann::json;

namespace eaframework {

std::unordered_map<std::string, ObjectiveFunctionType> objective_function_type_from_string = {
    {"MAXCUT", ObjectiveFunctionType::MAXCUT},
    {"MAXDICUT", ObjectiveFunctionType::MAXDICUT}
};

std::unordered_map<std::string, InformationCollectorType> information_collector_type_from_string = {
    {"default", InformationCollectorType::DEFAULT},
    {"iteration_data", InformationCollectorType::ITERATIONDATA},
};

static std::unordered_map<std::string, MutationOperatorType> mutation_operator_type_from_string = {
    {"default", MutationOperatorType::DEFAULT},
    {"unif", MutationOperatorType::UNIF},
    {"fmut", MutationOperatorType::FMUT},
    {"pmut", MutationOperatorType::PMUT},
    {"pmutActivity", MutationOperatorType::PMUTActivity},
    {"unifSigmoid", MutationOperatorType::UnifSigmoid},
};

template<typename T>
T get_value(const json& j, std::string field, T def) {
    if(j.find(field) != j.end()) {
        T value = j[field];
        return value;
    }
    else {
        return def;
    }
}

ExperimentConfig read_experiment_configuration(std::string path) {
    auto experiment_config = ExperimentConfig{};

    auto cfg_file = std::ifstream(path);
    json json_cfg;
    cfg_file >> json_cfg;

    experiment_config.generations = get_value(json_cfg, "generations", 1);
    experiment_config.time_limit = get_value(json_cfg, "time_limit", 0);
    auto info_type_string = get_value<std::string>(json_cfg, "information_collector_type", "default");
    experiment_config.information_collector_type = information_collector_type_from_string[info_type_string];
    experiment_config.run_count = get_value(json_cfg, "run_count", 1);
    auto objective_function_type_string = get_value<std::string>(json_cfg, "objective_function", "MAXCUT");
    experiment_config.objective_function_type = objective_function_type_from_string[objective_function_type_string];
    int start_type_raw = get_value(json_cfg, "start_type", 0);
    experiment_config.start_type = static_cast<StartType>(start_type_raw);

    for (auto &mutator : json_cfg["mutators"]) {
        auto cfg = MutationOperatorConfig{};
        auto mutation_operator_type_string = get_value<std::string>(mutator, "type", "default");
        cfg.type = mutation_operator_type_from_string[mutation_operator_type_string];
        cfg.id = get_value(mutator, "id", 0);
        if (mutator.find("arguments") != mutator.end()) {
            auto arguments = mutator["arguments"];
            for (json::iterator it = arguments.begin(); it != arguments.end(); ++it) {
                auto param = MutationOperatorParameter{};
                param.name = it.key();
                param.value = it.value();
                cfg.parameters.push_back(param);
            }
        }
        experiment_config.mutation_operator_configs.push_back(cfg);
    }

    return experiment_config;
}


}
