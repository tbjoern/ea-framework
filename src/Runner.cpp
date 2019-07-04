#include "Runner.hpp"

#include <tuple>
#include <memory>
#include <iostream>
#include <Instance.hpp>
#include <Experiment.hpp>
#include <InformationCollector.hpp>
#include <ObjectiveFunction.hpp>
#include <MutationOperatorFactory.hpp>
#include <EA.hpp>

namespace eaframework {

void execute_runs(std::string experiment_config_path, std::string instance_name) {
    auto experiment_config = read_experiment_configuration(experiment_config_path);
    auto instance = read_instance(instance_name);

    auto information_collector = build_information_collector(experiment_config.information_collector_type);
    auto objective_function = build_objective_function(experiment_config.objective_function_type, instance);
    auto mutation_operator_factory = MutationOperatorFactory(objective_function);

#pragma omp parallel for collapse(2)
    for(const auto& mutation_operator_config : experiment_config.mutation_operator_configs) {
        for(int i = 0; i < experiment_config.run_count; ++i) {
            auto mutation_operator = mutation_operator_factory.build(mutation_operator_config);
            auto ea = EA(objective_function, mutation_operator);
            ea.make_initial_individual(instance);
            for(int generation = 0; generation < experiment_config.generation_count; ++generation) {
                ea.next_generation();
                // information_collector->generation_snapshot(ea);
            }
        }
    }

    // information_collector->output_to_stream(std::cout);
}

}
