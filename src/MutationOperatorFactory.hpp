#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

class MutationOperator;

namespace eaframework {

class ObjectiveFunction;
class MutationOperatorConfig;

// we need multiple mutation operators with the same objective function
// thus this needs to be a class
class MutationOperatorFactory {
    std::shared_ptr<ObjectiveFunction> objective_function;
public:
    std::shared_ptr<MutationOperator> build(const MutationOperatorConfig&);
};


}
