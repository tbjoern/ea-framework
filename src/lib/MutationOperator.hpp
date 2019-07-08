#include <vector>

namespace eaframework {

struct Individual;

class MutationOperator {
    int id;
public:
    MutationOperator(int _id) : id(_id) {}

    virtual void setup_initial_individual(Individual&) {}
    virtual std::shared_ptr<Individual> mutate(const Individual&) = 0;
};

}
