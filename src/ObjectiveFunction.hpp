#include <vector>
#include <memory>

namespace eaframework {

class Instance;
struct Individual;

class ObjectiveFunction {
protected:
    const Instance& instance;
    ObjectiveFunction(const Instance& _instance) : instance(_instance) {}
public:
    virtual double operator()(Individual&) = 0;
};

enum class ObjectiveFunctionType {
    MAXCUT,
    MAXDICUT
};

// same paradigm as with InformationCollector factory
std::shared_ptr<ObjectiveFunction> build_objective_function(ObjectiveFunctionType, const Instance&);

}
