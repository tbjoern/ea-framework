#include "ObjectiveFunction.hpp"
#include <Instance.hpp>
#include <Graph.hpp>
#include <Individual.hpp>

namespace eaframework {

double ObjectiveFunction::evaluate(const Individual& individual) {
    ++call_count;
    return evaluate_impl(individual);
}

int ObjectiveFunction::callCount() const {
    return call_count;
}

int ObjectiveFunction::bitVectorSize() const {
    return instance.graph->node_count();
}

class TestFunction : public ObjectiveFunction {
public:
    TestFunction(const Instance& p_instance) : ObjectiveFunction(p_instance) {}

    double evaluate_impl(const Individual& individual) const override {
        return 0.0;
    }
};

class Maxcut : public ObjectiveFunction {
public:
    Maxcut(const Instance& p_instance) : ObjectiveFunction(p_instance) {}

    double evaluate_impl(const Individual& individual) const override {
        double cut_size = 0.0;
        const auto& edges = instance.graph->getEdges();
        const auto& bit_vector = individual.bit_vector;
        for(const auto& edge : edges) {
            if(bit_vector[edge.start] != bit_vector[edge.end] && bit_vector[edge.start] != BIT_NONE && bit_vector[edge.end] != BIT_NONE) {
                cut_size += edge.weight;
            }
        }
        return cut_size;
    }
};

class Maxdicut : public ObjectiveFunction {
public:
    Maxdicut(const Instance& p_instance) : ObjectiveFunction(p_instance) {}

    double evaluate_impl(const Individual& individual) const override {
        double cut_size = 0.0;
        const auto& bit_vector = individual.bit_vector;
        for(int node = 0; node < bit_vector.size(); ++node) {
            if(bit_vector[node] == BIT_ONE) {
                for(auto edge_ptr : instance.graph->getOutEdges(node)) {
                    if(bit_vector[edge_ptr->end] == BIT_ZERO) {
                        cut_size += edge_ptr->weight;
                    }
                }
            }
        }
        return cut_size;
    }
};

std::shared_ptr<ObjectiveFunction> build_objective_function(ObjectiveFunctionType type, const Instance& instance) {
    switch(type) {
        case ObjectiveFunctionType::TEST:
            return std::make_shared<TestFunction>(instance);
        case ObjectiveFunctionType::MAXCUT:
            return std::make_shared<Maxcut>(instance);
        case ObjectiveFunctionType::MAXDICUT:
            return std::make_shared<Maxdicut>(instance);
        default:
            throw std::invalid_argument("Objective function type not implemented");
    }
}

}
