#include "MutationOperator.hpp"

#include <Experiment.hpp>
#include <EA.hpp>
#include <ObjectiveFunction.hpp>
#include <activity_helpers.hpp>

#include <random>
#include <memory>
#include <map>
#include <functional>
#include <sstream>

#include <iostream>

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

        flip_count = 0;
        for (auto &bit : copy->bit_vector)
        {
            if ((*dist)(random_engine))
            {
                bit = !bit;
                ++flip_count;
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

        flip_count = 0;
        for (auto &bit : copy->bit_vector) 
        {
            if (dist(random_engine)) 
            {
                bit = !bit;
                ++flip_count;
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
        flip_count = bits_to_flip.size();
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
    std::shared_ptr<const Graph> graph;
public:
    PMUTActivity(Seed s, activity::Parameters _activity_params, double _power_law_beta) : MutationOperator(s), power_law_beta(_power_law_beta), activity_params(_activity_params) {}

    void setup_initial_individual(Individual& individual) override {
        auto bitCount = individual.bit_vector.size();

        activity::init(activity_params, individual);
        graph = activity::activityValues(bitCount, ea->getObjectiveFunction());

        p_gen = std::make_unique<PowerLawGenerator>(bitCount, power_law_beta);
    }

    std::shared_ptr<Individual> mutate(const Individual &parent) override
    {
        auto copy = std::make_shared<Individual>(parent);
        auto k = p_gen->get(random_engine);

        const auto& activity = parent.data_vectors.at("activity");
        auto dist = std::discrete_distribution<int>(activity.cbegin(), activity.cend());

        std::vector<Bit> bits_to_flip;
        while(k) {
            int bit = dist(random_engine);
            bits_to_flip.push_back(bit);
            --k;
        }
        flip_count = bits_to_flip.size();
        for(auto bit : bits_to_flip) {
            copy->bit_vector[bit] = !copy->bit_vector[bit];
        }

        activity::update(activity_params, *copy, bits_to_flip, *graph);
        activity::decay(activity_params, *copy);

        return copy;
    }
};

struct BernoulliGenerator {
  std::bernoulli_distribution b;
  std::mt19937& _engine;

  BernoulliGenerator(std::bernoulli_distribution _b, std::mt19937& _mt) : b(_b), _engine(_mt) {};

  bool get() { return b(_engine); }
};

double sigmoid(double x, double smoothness) {
    return 2 / (1 + exp(-1 * smoothness * x));
}

class UnifSigmoid : public MutationOperator {
    activity::Parameters activity_params;
    double sigmoid_smoothness;
    std::function<bool(int)> activity_sampler;
    std::shared_ptr<const Graph> graph;
public:
    UnifSigmoid(Seed s,activity::Parameters _activity_params, double _sigmoid_smoothness) : MutationOperator(s), activity_params(_activity_params), sigmoid_smoothness(_sigmoid_smoothness) {}

    void setup_initial_individual(Individual& individual) override {
        auto bitCount = individual.bit_vector.size();

        activity::init(activity_params, individual);
        graph = activity::activityValues(bitCount, ea->getObjectiveFunction());

        activity_sampler = build_activity_sigmoid_sampler(bitCount);
    }

    std::function<bool(int)> build_activity_sigmoid_sampler(int bitCount) {
        std::unordered_map<int, BernoulliGenerator> act_to_sampler;
        double max_p = 1.0 / 2, center_p = 1.0 / bitCount,
                min_p = 1.0 / (bitCount * bitCount);
        double upper_multiplier = max_p - center_p;
        double lower_multiplier = center_p - min_p;
        for (int activity = activity_params.min; activity <= activity_params.max;
            ++activity) {
            double sigmoid_value = sigmoid(activity, sigmoid_smoothness);
            double probability = 0.0;
            if (sigmoid_value > 1) {
            sigmoid_value -= 1;
            probability = upper_multiplier * sigmoid_value + center_p;
            } else {
            probability = lower_multiplier * sigmoid_value + min_p;
            }
            act_to_sampler.emplace(activity, BernoulliGenerator(std::bernoulli_distribution(probability), random_engine));
        }
        return [act_to_sampler](double activity) mutable {
            return act_to_sampler.at(static_cast<int>(ceil(activity))).get();
        };
    }

    std::shared_ptr<Individual> mutate(const Individual &parent) override
    {
        auto copy = std::make_shared<Individual>(parent);

        const auto& activity = parent.data_vectors.at("activity");

        std::vector<Bit> bits_to_flip;
        for(int bit = 0; bit < copy->bit_vector.size(); ++bit) {
            if(activity_sampler(activity[bit])) {
                bits_to_flip.push_back(bit);
            }
        }
        flip_count = bits_to_flip.size();
        for(auto bit : bits_to_flip) {
            copy->bit_vector[bit] = !copy->bit_vector[bit];
        }

        activity::update(activity_params, *copy, bits_to_flip, *graph);
        activity::decay(activity_params, *copy);

        return copy;
    }
};

activity::Parameters parse_activity_parameters(const MutationOperatorConfig &config) {
    activity::Parameters params;
    for(const auto& param : config.parameters) {
        if(param.name == "inc") {
            params.inc = param.value;
            continue;
        }
        if(param.name == "dec") {
            params.dec = param.value;
            continue;
        }
        if(param.name == "max") {
            params.max = param.value;
            continue;
        }
        if(param.name == "min") {
            params.min = param.value;
            continue;
        }
        if(param.name == "start") {
            params.start = param.value;
            continue;
        }
        if(param.name == "decay_rate") {
            params.decay_rate = param.value;
            continue;
        }
    }
    return params;
}

const MutationOperatorParameter& find_param(std::string name, const MutationOperatorConfig &config) {
    for(const auto& param : config.parameters) {
        if(param.name == name) {
            return param;
        }
    }
    std::stringstream s;
    s << "Could not find parameter " << name << std::endl;
    throw std::runtime_error(s.str());
}

std::shared_ptr<MutationOperator> build_mutation_operator(const MutationOperatorConfig &config)
{
    Seed s = ::rand();
    // std::cout << s << std::endl;
    double power_law_beta, sigmoid_smoothness;
    activity::Parameters params;
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
    case MutationOperatorType::PMUTActivity:
        params = parse_activity_parameters(config);
        power_law_beta = find_param("power_law_beta", config).value;
        return std::make_shared<PMUTActivity>(s, params, power_law_beta);
    case MutationOperatorType::UnifSigmoid:
        params = parse_activity_parameters(config);
        sigmoid_smoothness = find_param("sigmoid_smoothness", config).value;
        return std::make_shared<UnifSigmoid>(s, params, sigmoid_smoothness);
    default:
        throw std::invalid_argument("Unknown Operator Type");
    }
}

} // namespace eaframework
