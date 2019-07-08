#include <vector>
#include <memory>

namespace eaframework {

class MutationOperator;
class ObjectiveFunction;
struct Individual;
class Instance;

class EA {
private:
    std::shared_ptr<const Individual> best_individual;
    std::shared_ptr<const Individual> offspring;
    std::shared_ptr<MutationOperator> mutator;
    std::shared_ptr<ObjectiveFunction> objective_function;
    bool generation_improved;
    double mutation_time;
    double best_fitness_value;
    double offspring_fitness_value;

public:
    const MutationOperator& getMutator() const;
    
    EA(std::shared_ptr<ObjectiveFunction>, std::shared_ptr<MutationOperator>);

    void make_initial_individual(Instance&);

    void next_generation();

    const Individual& getBestIndividual() const;
    const Individual& getOffspring() const;
    const ObjectiveFunction& getObjectiveFunction() const;
    double getMutationTime() const;
    double getBestFitness() const;
    double getOffspringFitness() const;
    bool generationImproved() const;
};

}
