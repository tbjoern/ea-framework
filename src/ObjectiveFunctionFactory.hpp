#include <memory>

class ObjectiveFunction;

namespace eaframework {

class Instance;

enum class ObjectiveFunctionType {
    MAXCUT,
    MAXDICUT
};

class ObjectiveFunctionFactory {
    std::shared_ptr<ObjectiveFunction> build(ObjectiveFunctionType, std::shared_ptr<Instance>);
};

}
