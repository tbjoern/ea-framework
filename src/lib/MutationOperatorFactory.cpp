#include "MutationOperatorFactory.hpp"

#include <MutationOperator.hpp>
#include <Individual.hpp>
#include <random>
#include <Experiment.hpp>

namespace eaframework {

class DummyOperator : public MutationOperator {
    std::mt19937 _engine;
    std::bernoulli_distribution _dist;
public:
    DummyOperator(int _id) : MutationOperator(_id), _engine(std::random_device{}()), _dist(0.5) {}

    std::shared_ptr<Individual> mutate(const Individual& parent) override {
        auto copy = Individual(parent);

        for(auto& bit : copy.bit_vector) {
            bit = BIT_ZERO;
        }

        copy.bit_vector[0] = BIT_ONE;

        return std::make_shared<Individual>(std::move(copy));
    }
};

std::shared_ptr<MutationOperator> MutationOperatorFactory::build(const MutationOperatorConfig& config) {
    return std::make_shared<DummyOperator>(config.id);
}

}
