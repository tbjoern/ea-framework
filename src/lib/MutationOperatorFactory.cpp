#include "MutationOperatorFactory.hpp"

#include <MutationOperator.hpp>
#include <Individual.hpp>

namespace eaframework {

class DummyOperator : public MutationOperator {
public:
    std::shared_ptr<Individual> mutate(const Individual& parent) override {
        auto copy = Individual(parent);
        return std::make_shared<Individual>(copy);
    }
};

std::shared_ptr<MutationOperator> MutationOperatorFactory::build(const MutationOperatorConfig&) {
    return std::make_shared<DummyOperator>();
}

}
