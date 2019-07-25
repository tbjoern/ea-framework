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

void EA::make_initial_individual(const Instance& instance) {
    auto start_assignment_copy = Individual(*(instance.start_assignment));
    mutator->setup_initial_individual(start_assignment_copy);
    best_individual = std::make_shared<Individual>(std::move(start_assignment_copy));
}

EA::EA(std::shared_ptr<ObjectiveFunction> _objective_function, std::shared_ptr<MutationOperator> _mutator) : best_individual(nullptr), offspring(nullptr), mutator(_mutator), objective_function(_objective_function), generation_improved(false), best_fitness(0), offspring_fitness(0) {
    _mutator->setEA(this);
}

void EA::next_generation() {
    auto start_time = std::chrono::high_resolution_clock::now();
    offspring = mutator->mutate(*best_individual);
    auto stop_time = std::chrono::high_resolution_clock::now();
    mutation_time = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time).count();

    generation_improved = false;
    offspring_fitness = objective_function->evaluate(*offspring);
    if(best_fitness < offspring_fitness) {
        best_individual = offspring;
        best_fitness = offspring_fitness;
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

ObjectiveFunction& EA::getObjectiveFunction() const {
    return *objective_function;
}

int EA::getBestFitness() const {
    return best_fitness;
}

int EA::getOffspringFitness() const {
    return offspring_fitness;
}

bool EA::generationImproved() const {
    return generation_improved;
}

double EA::getMutationTime() const {
    return mutation_time;
}

}
