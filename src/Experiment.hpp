#include <string>
#include <vector>
#include <string>
#include <memory>

namespace eaframework {

class Instance;
enum class ObjectiveFunctionType;

struct MutationOperatorParameter {
    std::string name;
    union value {
        int i;
        double d;
        bool b;
    };
};

enum class MutationOperatorType {
    DEFAULT
};

struct MutationOperatorConfig {
    MutationOperatorType type;
    std::vector<MutationOperatorParameter> parameters;
};

struct Run {
    std::shared_ptr<Instance> instance;
    std::shared_ptr<ExperimentConfig> experiment_config;
    std::shared_ptr<MutationOperatorConfig> mutation_operator_config;
};

struct ExperimentConfig {
    int run_count;
    unsigned long long computation_time_budget;
    bool use_predefined_start;
    ObjectiveFunctionType objective_function_type;
    int generation_count;
};

std::tuple<std::shared_ptr<ExperimentConfig>, std::vector<std::shared_ptr<MutationOperatorConfig>>> read_experiment_configuration(std::string);

}
