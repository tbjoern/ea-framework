#pragma once
#include <memory>
#include <vector>
#include <string>

namespace eaframework {

struct Run;
class ExperimentConfig;

void execute_runs(std::string experiment_config_path, std::string instance_name);

}
