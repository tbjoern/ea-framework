#include <memory>

namespace eaframework {

struct Run;
enum class InformationCollectorType;

void execute_runs(std::vector<Run>& runs, InformationCollectorType);

}
