#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

class MutationOperator;

namespace eaframework {

class ObjectiveFunction;
class MutationOperatorConfig;

class MutationOperatorFactory {
    std::shared_ptr<ObjectiveFunction> objective_function;

    std::shared_ptr<MutationOperator> build(const MutationOperatorConfig&);
};


}
