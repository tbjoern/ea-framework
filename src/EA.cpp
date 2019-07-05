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
    parent = std::make_shared<Individual>(start_assignment_copy);
}

EA::EA(std::shared_ptr<ObjectiveFunction> _objective_function, std::shared_ptr<MutationOperator> _mutator) : parent(nullptr), previous_parent(nullptr), mutator(_mutator), objective_function(_objective_function), generation_improved(false) {}

void EA::next_generation() {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto offspring = mutator->mutate(*parent);
    auto stop_time = std::chrono::high_resolution_clock::now();
    mutation_time = (stop_time - start_time).count();

    previous_parent = parent;
    if(objective_function->evaluate(*parent) < objective_function->evaluate(*offspring)) {
        parent = offspring;
    }
}

const Individual& EA::getBestIndividual() const {
    return *parent;
}
const Individual& EA::getPreviousIndividual() const {
    return *previous_parent;
}

const MutationOperator& EA::getMutator() const {
    return *mutator;
}

}
