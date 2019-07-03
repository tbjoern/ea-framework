#include <vector>
#include <memory>
#include <typedefs.hpp>

class MutationOperator;
class ObjectiveFunction;

namespace eaframework {

class EA {
private:
    Individual parent;
    Individual previous_parent;
    std::unique_ptr<MutationOperator> mutator;
    std::shared_ptr<const ObjectiveFunction> objective_function;
    bool generation_improved;

public:
    const MutationOperator& getMutator() const;
    
    EA(std::shared_ptr<ObjectiveFunction>, MutationOperator&&);

    void next_generation();

    const Individual& getBestIndividual() const;
    const Individual& getPreviousIndividual() const;
};

}
