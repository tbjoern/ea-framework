#include "MutationOperatorFactory.hpp"

#include <MutationOperator.hpp>

namespace eaframework {

class DummyOperator : public MutationOperator {
public:
    Individual mutate(Individual& parent) override {
        return parent;
    }
};

std::shared_ptr<MutationOperator> MutationOperatorFactory::build(const MutationOperatorConfig&) {
    return std::make_shared<DummyOperator>();
}

}
