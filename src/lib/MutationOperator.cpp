#include "MutationOperator.hpp"

#include <Individual.hpp>
#include <Experiment.hpp>
#include <EA.hpp>
#include <ObjectiveFunction.hpp>

#include <random>
#include <memory>

namespace {
    std::random_device rand;
}

namespace eaframework {

class DummyOperator : public MutationOperator {
public:
    DummyOperator(int _id, Seed s) : MutationOperator(_id, s) {}

    std::shared_ptr<Individual> mutate(const Individual& parent) override {
        auto copy = Individual(parent);

        for(auto& bit : copy.bit_vector) {
            bit = BIT_ZERO;
        }

        copy.bit_vector[0] = BIT_ONE;

        return std::make_shared<Individual>(std::move(copy));
    }
};

class Unif : public MutationOperator {
    std::unique_ptr<std::bernoulli_distribution> dist;
public:
    Unif(int id, Seed s) : MutationOperator(id, s) {}

    void setup_initial_individual(Individual&) override {
        int bitCount = ea->getObjectiveFunction().bitVectorSize();
        dist = std::make_unique<std::bernoulli_distribution>(1.0 / bitCount);
    }

    std::shared_ptr<Individual> mutate(const Individual& parent) override {
        auto copy = std::make_shared<Individual>(parent);

        for(auto& bit : copy->bit_vector) {
            if((*dist)(random_engine)) {
                bit = !bit;
            }
        }
        return copy;
    }
};

std::shared_ptr<MutationOperator> build_mutation_operator(const MutationOperatorConfig& config) {
    Seed s = ::rand();
    switch(config.type) {
        case MutationOperatorType::DEFAULT:
            return std::make_shared<DummyOperator>(config.id, s);
        case MutationOperatorType::UNIF:
            return std::make_shared<Unif>(config.id, s);
        default:
            throw std::invalid_argument("Unknown Operator Type");
    }
}

}
