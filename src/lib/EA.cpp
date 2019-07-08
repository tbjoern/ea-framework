#include "EA.hpp"

#include <ObjectiveFunction.hpp>
#include <Instance.hpp>
#include <MutationOperator.hpp>
#include <Individual.hpp>
#include <chrono>

namespace eaframework {

// class EA {
// private:
//     std::shared_ptr<const Individual> parent;
//     std::shared_ptr<const Individual> previous_parent;
//     std::shared_ptr<MutationOperator> mutator;
//     std::shared_ptr<const ObjectiveFunction> objective_function;
//     bool generation_improved;

// public:
//     const MutationOperator& getMutator() const;
    
//     EA(std::shared_ptr<ObjectiveFunction>, std::shared_ptr<MutationOperator>);

//     void make_initial_individual(std::shared_ptr<Instance>);
//     void next_generation();

//     const Individual& getBestIndividual() const;
//     const Individual& getPreviousIndividual() const;
// };

void EA::make_initial_individual(Instance& instance) {
    auto start_assignment_copy = Individual(*(instance.start_assignment));
    mutator->setup_initial_individual(start_assignment_copy);
    best_individual = std::make_shared<Individual>(std::move(start_assignment_copy));
}

EA::EA(std::shared_ptr<ObjectiveFunction> _objective_function, std::shared_ptr<MutationOperator> _mutator) : best_individual(nullptr), offspring(nullptr), mutator(_mutator), objective_function(_objective_function), generation_improved(false) {
    _mutator->setEA(this);
}

void EA::next_generation() {
    auto start_time = std::chrono::high_resolution_clock::now();
    offspring = mutator->mutate(*best_individual);
    auto stop_time = std::chrono::high_resolution_clock::now();
    mutation_time = (stop_time - start_time).count();

    generation_improved = false;
    double offspring_fitness = objective_function->evaluate(*offspring);
    if(best_fitness_value < offspring_fitness) {
        best_individual = offspring;
        best_fitness_value = offspring_fitness;
        generation_improved = true;
    }
}

const Individual& EA::getBestIndividual() const {
    return *best_individual;
}
const Individual& EA::getOffspring() const {
    return *offspring;
}

const MutationOperator& EA::getMutator() const {
    return *mutator;
}

const ObjectiveFunction& EA::getObjectiveFunction() const {
    return *objective_function;
}

double EA::getBestFitness() const {
    return best_fitness_value;
}

double EA::getOffspringFitness() const {
    return offspring_fitness_value;
}

bool EA::generationImproved() const {
    return generation_improved;
}

}
