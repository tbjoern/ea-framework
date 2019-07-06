#include "MutationOperatorFactory.hpp"

#include <MutationOperator.hpp>
#include <Individual.hpp>
#include <random>

namespace eaframework {

class DummyOperator : public MutationOperator {
    std::mt19937 _engine;
    std::bernoulli_distribution _dist;
public:
    DummyOperator() : _engine(std::random_device{}()), _dist(0.5) {}

    std::shared_ptr<Individual> mutate(const Individual& parent) override {
        auto copy = Individual(parent);

        for(int i = 0; i < copy.bit_vector.size(); ++i) {
            if(_dist(_engine)) {
                copy.bit_vector[i] = copy.bit_vector[i] != 1;
            }
        }

        return std::make_shared<Individual>(copy);
    }
};

std::shared_ptr<MutationOperator> MutationOperatorFactory::build(const MutationOperatorConfig&) {
    return std::make_shared<DummyOperator>();
}

}
