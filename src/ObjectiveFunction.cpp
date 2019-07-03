#include "ObjectiveFunction.hpp"

namespace eaframework {

class DummyFunction : public ObjectiveFunction {
public:
    DummyFunction(const Instance& p_instance) : ObjectiveFunction(p_instance) {}

    double operator()(Individual) override {
        return 0.0;
    }
};

std::shared_ptr<ObjectiveFunction> build_objective_function(ObjectiveFunctionType, const Instance& instance) {
    return std::make_shared<DummyFunction>(instance);
}

}
