#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace eaframework {

class MutationOperator;
class ObjectiveFunction;
class MutationOperatorConfig;

enum class MutationOperatorType {
    DEFAULT
};

// we need multiple mutation operators with the same objective function
// thus this needs to be a class
class MutationOperatorFactory {
    std::shared_ptr<ObjectiveFunction> objective_function;
public:
    MutationOperatorFactory(std::shared_ptr<ObjectiveFunction>& _objective_function) : objective_function(_objective_function) {}

    std::shared_ptr<MutationOperator> build(const MutationOperatorConfig&);
};


}
