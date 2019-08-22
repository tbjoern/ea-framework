#include "Runner.hpp"

#include <tuple>
#include <memory>
#include <iostream>
#include <Instance.hpp>
#include <Experiment.hpp>
#include <InformationCollector.hpp>
#include <ObjectiveFunction.hpp>
#include <MutationOperator.hpp>
#include <EA.hpp>
#include <mutex>
#include <chrono>
#include <omp.h>

namespace {
    int64_t time(std::chrono::_V2::system_clock::time_point start) {
        auto cur_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(cur_time - start).count();
    }
}

namespace eaframework {

void execute_runs(std::string experiment_config_path, std::string instance_name) {
    const auto experiment_config = read_experiment_configuration(experiment_config_path);
    const auto instance = read_instance(instance_name, experiment_config.start_type, experiment_config.objective_function_type);

    auto information_collector = build_information_collector(experiment_config.information_collector_type);

    std::mutex m;

    information_collector->write_header(std::cout);

    const int64_t time_limit = experiment_config.time_limit * 60 * 1000; // from min to ms
    auto since_start = std::chrono::high_resolution_clock::now();

    const auto config_count = experiment_config.mutation_operator_configs.size();
    const auto run_count = experiment_config.run_count;
#pragma omp parallel for collapse(2)
    for(int c = 0; c < config_count; ++c) {
        for(int run = 0; run < run_count; ++run) {
            const auto& mutation_operator_config = experiment_config.mutation_operator_configs[c];
            int id = mutation_operator_config.id;
            auto mutation_operator = build_mutation_operator(mutation_operator_config);
            auto objective_function = build_objective_function(experiment_config.objective_function_type, instance);
            auto ea = EA(objective_function, mutation_operator);
            ea.make_initial_individual(instance);
            for(int generation = 0; generation < experiment_config.generations && time(since_start) < time_limit; ++generation) {
                ea.next_generation();
                std::lock_guard<std::mutex> l(m);
                information_collector->generation_snapshot(id, run, generation, ea);
            }
        }
    }

    information_collector->output_to_stream(std::cout);
}

}
