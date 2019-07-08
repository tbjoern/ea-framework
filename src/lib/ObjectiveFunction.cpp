#include "ObjectiveFunction.hpp"
#include <Instance.hpp>
#include <Graph.hpp>
#include <Individual.hpp>

namespace eaframework {

class DummyFunction : public ObjectiveFunction {
public:
    DummyFunction(const Instance& p_instance) : ObjectiveFunction(p_instance) {}

    double evaluate(const Individual& individual) const override {
        return 0.0;
    }
};

class Maxcut : public ObjectiveFunction {
public:
    Maxcut(const Instance& p_instance) : ObjectiveFunction(p_instance) {}

    double evaluate(const Individual& individual) const override {
        double cut_size = 0.0;
        const auto& edges = instance.graph->getEdges();
        const auto& bit_vector = individual.bit_vector;
        for(const auto& edge : edges) {
            if(bit_vector[edge.start] != bit_vector[edge.end]) {
                cut_size += edge.weight;
            }
        }
        return cut_size;
    }
};

std::shared_ptr<ObjectiveFunction> build_objective_function(ObjectiveFunctionType type, const Instance& instance) {
    switch(type) {
        case ObjectiveFunctionType::MAXCUT:
            return std::make_shared<Maxcut>(instance);
    }
    return std::make_shared<DummyFunction>(instance);
}

}
