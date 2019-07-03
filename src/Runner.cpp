#include "Runner.hpp"

#include <tuple>
#include <memory>
#include <Instance.hpp>
#include <Experiment.hpp>

namespace eaframework {

void run_experiment(std::string experiment_config_path, std::string instance_name) {
    auto config_output = read_experiment_configuration(experiment_config_path);
    auto experiment_config = std::get<0>(config_output);
    auto mutation_operator_configs = std::get<1>(config_output);
    auto instance = std::make_shared<Instance>(read_instance(instance_name));

    // generate Run configurations

    std::vector<Run> runs;
    for(auto mutation_operator_config : mutation_operator_configs) {
        for(int i = 0; i < experiment_config->run_count; ++i) {
            Run run {instance, experiment_config, mutation_operator_config};
            runs.push_back(run);
        }
    }

    execute_runs(runs, experiment_config->information_collector_type);
}

void execute_runs(std::vector<Run>& runs, InformationCollectorType) {

}

}
