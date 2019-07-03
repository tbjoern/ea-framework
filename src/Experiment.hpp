#include <string>
#include <vector>
#include <string>
#include <memory>

namespace eaframework {

class Instance;
enum class ObjectiveFunctionType;
enum class InformationCollectorType;

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


struct ExperimentConfig {
    int run_count;
    unsigned long long computation_time_budget;
    bool use_predefined_start;
    ObjectiveFunctionType objective_function_type;
    InformationCollectorType information_collector_type;
    int generation_count;
    std::shared_ptr<const Instance> instance;
    std::vector<MutationOperatorConfig> mutation_operator_configs;
};

ExperimentConfig read_experiment_configuration(std::string);

}
