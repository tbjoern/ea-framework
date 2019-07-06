#include <vector>

namespace eaframework {

struct Individual;

class MutationOperator {
public:
    virtual void setup_initial_individual(Individual&) {}
    virtual std::shared_ptr<Individual> mutate(const Individual&) = 0;
};

}
