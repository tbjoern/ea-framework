#include <memory>
#include <vector>
#include <string>

namespace eaframework {

struct Run;
enum class InformationCollectorType;

void run_experiment(std::string experiment_config_path, std::string instance_name);

void execute_runs(std::vector<Run>& runs, InformationCollectorType);

}
