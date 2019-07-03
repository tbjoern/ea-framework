#include "Experiment.hpp"

namespace eaframework {

std::tuple<std::shared_ptr<ExperimentConfig>, std::vector<std::shared_ptr<MutationOperatorConfig>>> read_experiment_configuration(std::string) {
    auto experiment_config = std::make_shared<ExperimentConfig>();

    auto mutation_operator_configs = std::vector<std::shared_ptr<MutationOperatorConfig>>();

    return std::make_tuple(experiment_config, mutation_operator_configs);
}


}
