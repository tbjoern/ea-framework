#include <vector>
#include <typedefs.hpp>

namespace eaframework {

class MutationOperator {
public:
    virtual Individual mutate(Individual) = 0;
};

}
