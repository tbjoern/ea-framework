#include <vector>
#include <memory>

namespace eaframework {

class MutationOperator;
class ObjectiveFunction;
struct Individual;
class Instance;

class EA {
private:
    std::shared_ptr<const Individual> parent;
    std::shared_ptr<const Individual> previous_parent;
    std::shared_ptr<MutationOperator> mutator;
    std::shared_ptr<const ObjectiveFunction> objective_function;
    bool generation_improved;
    double mutation_time;

public:
    const MutationOperator& getMutator() const;
    
    EA(std::shared_ptr<ObjectiveFunction>, std::shared_ptr<MutationOperator>);

    void make_initial_individual(Instance&);

    void next_generation();

    const Individual& getBestIndividual() const;
    const Individual& getPreviousIndividual() const;
    const ObjectiveFunction& getObjectiveFunction() const;
    double getMutationTime() const;
    bool generationImproved() const;
};

}
