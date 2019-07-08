#include <vector>
#include <memory>

namespace eaframework {

struct Individual;
class EA;
struct MutationOperatorConfig;

class MutationOperator {
    int id;
    EA* ea;
public:
    MutationOperator(int _id) : id(_id) {}

    void setEA(EA* _ea) { ea = _ea; }
    virtual void setup_initial_individual(Individual&) {}
    virtual std::shared_ptr<Individual> mutate(const Individual&) = 0;
};

enum class MutationOperatorType {
    DEFAULT
};

std::shared_ptr<MutationOperator> build_mutation_operator(const MutationOperatorConfig&);


}
