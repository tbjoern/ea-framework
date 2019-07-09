#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace eaframework {

struct Instance;
struct Individual;

class ObjectiveFunction {
    int call_count;
protected:
    const Instance& instance;
    ObjectiveFunction(const Instance& _instance) : instance(_instance), call_count(0) {}

    virtual double evaluate_impl(const Individual&) const = 0;
public:
    double evaluate(const Individual&);
    int callCount() const;
    int bitVectorSize() const;
};

enum class ObjectiveFunctionType {
    TEST,
    MAXCUT,
    MAXDICUT
};

std::shared_ptr<ObjectiveFunction> build_objective_function(ObjectiveFunctionType type, const Instance& instance);

}
