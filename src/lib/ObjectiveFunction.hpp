#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace eaframework {

class Instance;
struct Individual;

class ObjectiveFunction {
    int call_count;
protected:
    const Instance& instance;
    ObjectiveFunction(const Instance& _instance) : instance(_instance) {}

    virtual double evaluate_impl(const Individual&) const = 0;
public:
    double evaluate(const Individual&);
    int callCount() const;
};

enum class ObjectiveFunctionType {
    TEST,
    MAXCUT,
    MAXDICUT
};


// same paradigm as with InformationCollector factory
std::shared_ptr<ObjectiveFunction> build_objective_function(ObjectiveFunctionType, const Instance&);

}
