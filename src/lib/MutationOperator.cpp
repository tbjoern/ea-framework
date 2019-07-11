#include "MutationOperator.hpp"

#include <Experiment.hpp>
#include <EA.hpp>
#include <ObjectiveFunction.hpp>
#include <activity_helpers.hpp>

#include <random>
#include <memory>
#include <map>

namespace
{
std::random_device rand;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

struct PowerLawGenerator
{
    std::vector<double> VWeights;
    std::map<double, int> TV;
    std::uniform_real_distribution<double> dist;

    PowerLawGenerator(int variables, double beta)
    {
        VWeights.resize(variables);
        double VSum = 0;

        // calculating the exponents for power-law weights
        double betaNorm = 1 / (beta - 1);

        // mapping variables to weights and creating a 'cumulative distribution'
        for (int i = 0; i < variables; ++i)
        {
            VWeights[i] = pow((variables / (i + 1)), betaNorm);
            VSum += VWeights[i];
            TV[VSum] = i;
        }

        dist = std::uniform_real_distribution<double>(0.0, VSum);
    }

    int get(std::mt19937 &engine)
    {
        double v = dist(engine);
        auto var = TV.lower_bound(v)->second;
        return var;
    }
};
} // namespace

namespace eaframework
{

class DummyOperator : public MutationOperator
{
public:
    DummyOperator(Seed s) : MutationOperator(s) {}

    std::shared_ptr<Individual> mutate(const Individual &parent) override
    {
        auto copy = std::make_shared<Individual>(parent);

        for (auto &bit : copy->bit_vector)
        {
            bit = BIT_ZERO;
        }

        copy->bit_vector[0] = BIT_ONE;

        return copy;
    }
};

class Unif : public MutationOperator
{
    std::unique_ptr<std::bernoulli_distribution> dist;

public:
    Unif(Seed s) : MutationOperator(s) {}

    void setup_initial_individual(Individual & individual) override
    {
        int bitCount = individual.bit_vector.size();
        dist = std::make_unique<std::bernoulli_distribution>(1.0 / bitCount);
    }

    std::shared_ptr<Individual> mutate(const Individual &parent) override
    {
        auto copy = std::make_shared<Individual>(parent);

        for (auto &bit : copy->bit_vector)
        {
            if ((*dist)(random_engine))
            {
                bit = !bit;
            }
        }
        return copy;
    }
};

class FMUT : public MutationOperator
{
    std::unique_ptr<PowerLawGenerator> p_gen;
    double power_law_beta;
public:
    FMUT(Seed s, double _power_law_beta) : MutationOperator(s), power_law_beta(_power_law_beta) {}

    void setup_initial_individual(Individual& individual) override {
        auto bitCount = individual.bit_vector.size();
        p_gen = std::make_unique<PowerLawGenerator>(bitCount/2.0, power_law_beta);
    }

    std::shared_ptr<Individual> mutate(const Individual &parent) override
    {
        auto copy = std::make_shared<Individual>(parent);
        auto bitCount = copy->bit_vector.size();
        double alpha = p_gen->get(random_engine);
        auto dist = std::bernoulli_distribution(alpha/bitCount);

        for (auto &bit : copy->bit_vector) 
        {
            if (dist(random_engine)) 
            {
                bit = !bit;
            }
        }
        return copy;
    }
};

class PMUT : public MutationOperator
{
    std::unique_ptr<PowerLawGenerator> p_gen;
    std::unique_ptr<std::uniform_int_distribution<>> int_dist;
    double power_law_beta;
    int bitCount;
public:
    PMUT(Seed s, double _power_law_beta) : MutationOperator(s), power_law_beta(_power_law_beta) {}

    void setup_initial_individual(Individual& individual) override {
        bitCount = individual.bit_vector.size();
        p_gen = std::make_unique<PowerLawGenerator>(bitCount, power_law_beta);
        int_dist = std::make_unique<std::uniform_int_distribution<>>(0, bitCount-1);
    }

    std::shared_ptr<Individual> mutate(const Individual &parent) override
    {
        auto copy = std::make_shared<Individual>(parent);
        auto k = p_gen->get(random_engine);

        std::vector<int> bits_to_flip;
        while(k) {
            int bit = (*int_dist)(random_engine);
            bits_to_flip.push_back(bit);
            --k;
        }
        for(auto bit : bits_to_flip) {
            copy->bit_vector[bit] = !copy->bit_vector[bit];
        }

        return copy;
    }
};

class PMUTActivity : public MutationOperator {
    double power_law_beta;
    activity::Parameters activity_params;
    std::unique_ptr<PowerLawGenerator> p_gen;
public:
    PMUTActivity(Seed s, double _power_law_beta, activity::Parameters _activity_params) : MutationOperator(s), power_law_beta(_power_law_beta), activity_params(_activity_params) {}

    void setup_initial_individual(Individual& individual) override {
        auto bitCount = individual.bit_vector.size();
        p_gen = std::make_unique<PowerLawGenerator>(bitCount, power_law_beta);

        activity::init(activity_params, individual);
    }

    std::shared_ptr<Individual> mutate(const Individual &parent) override
    {
        auto copy = std::make_shared<Individual>(parent);
        auto k = p_gen->get(random_engine);

        const auto& activity = parent.data_vectors.at("activity");
        auto dist = std::discrete_distribution<int>(activity.cbegin(), activity.cend());

        std::vector<int> bits_to_flip;
        while(k) {
            int bit = dist(random_engine);
            bits_to_flip.push_back(bit);
            --k;
        }
        for(auto bit : bits_to_flip) {
            copy->bit_vector[bit] = !copy->bit_vector[bit];
        }

        return copy;
    }
};

std::shared_ptr<MutationOperator> build_mutation_operator(const MutationOperatorConfig &config)
{
    Seed s = ::rand();
    switch (config.type)
    {
    case MutationOperatorType::DEFAULT:
        return std::make_shared<DummyOperator>(s);
    case MutationOperatorType::UNIF:
        return std::make_shared<Unif>(s);
    case MutationOperatorType::FMUT:
        return std::make_shared<FMUT>(s, config.parameters[0].value);
    case MutationOperatorType::PMUT:
        return std::make_shared<PMUT>(s, config.parameters[0].value);
    default:
        throw std::invalid_argument("Unknown Operator Type");
    }
}

} // namespace eaframework
