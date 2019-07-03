#include "Runner.hpp"

#include <tuple>
#include <memory>
#include <Instance.hpp>
#include <Experiment.hpp>
#include <InformationCollector.hpp>
#include <ObjectiveFunction.hpp>

namespace eaframework {

void execute_runs(std::string experiment_config_path, std::string instance_name) {
    auto experiment_config = read_experiment_configuration(experiment_config_path);
    auto instance = read_instance(instance_name);

    auto information_collector = build_information_collector(experiment_config.information_collector_type);
    auto objective_function = build_objective_function(experiment_config.objective_function_type, instance);

#pragma omp parallel for collapse(2)
    for(const auto& mutation_operator_config : experiment_config.mutation_operator_configs) {
        for(int i = 0; i < experiment_config.run_count; ++i) {
            
        }
    }
}

}
