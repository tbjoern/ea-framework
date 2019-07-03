#include <vector>
#include <memory>
#include <typedefs.hpp>

namespace eaframework {

class Instance;

class ObjectiveFunction {
    std::shared_ptr<Instance> instance;
public:
    virtual double operator()(Individual) = 0;
};

enum class ObjectiveFunctionType {
    MAXCUT,
    MAXDICUT
};

// same paradigm as with InformationCollector factory
std::shared_ptr<ObjectiveFunction> build_objective_function(ObjectiveFunctionType, std::shared_ptr<Instance>);

}
