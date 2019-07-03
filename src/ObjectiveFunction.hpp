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

}
