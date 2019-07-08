#include <vector>
#include <memory>
#include <random>

namespace eaframework {

struct Individual;
class EA;
struct MutationOperatorConfig;

typedef unsigned long long Seed;

class MutationOperator {
protected:
    std::mt19937 random_engine;
    EA* ea;
public:
    MutationOperator(Seed s) : random_engine(s) {}

    void setEA(EA* _ea) { ea = _ea; }
    virtual void setup_initial_individual(Individual&) {}
    virtual std::shared_ptr<Individual> mutate(const Individual&) = 0;
};

enum class MutationOperatorType {
    DEFAULT,
    UNIF
};

std::shared_ptr<MutationOperator> build_mutation_operator(const MutationOperatorConfig&);


}
