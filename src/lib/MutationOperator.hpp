#pragma once
#include <vector>
#include <memory>
#include <random>

namespace eaframework {

struct Individual;
class EA;
struct MutationOperatorConfig;

typedef unsigned long long Seed;

class MutationOperator {
protected:
    std::mt19937 random_engine;
    EA* ea;
    int flip_count;
public:
    MutationOperator(Seed s) : random_engine(s) {}

    void setEA(EA* _ea) { ea = _ea; }
    virtual void setup_initial_individual(Individual&) {}
    virtual std::shared_ptr<Individual> mutate(const Individual&) = 0;
    virtual int get_flip_count() const {
        return flip_count;
    }
};

enum class MutationOperatorType {
    DEFAULT,
    UNIF,
    FMUT,
    PMUT,
    PMUTActivity,
    UnifSigmoid,
};

std::shared_ptr<MutationOperator> build_mutation_operator(const MutationOperatorConfig&);


}
