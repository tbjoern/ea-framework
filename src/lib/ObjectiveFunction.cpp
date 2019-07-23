#include "ObjectiveFunction.hpp"
#include <Instance.hpp>
#include <Graph.hpp>
#include <Individual.hpp>

namespace eaframework {

int ObjectiveFunction::evaluate(const Individual& individual) {
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

    int evaluate_impl(const Individual& individual) const override {
        return 0;
    }
};

class Maxcut : public ObjectiveFunction {
public:
    Maxcut(const Instance& p_instance) : ObjectiveFunction(p_instance) {}

    int evaluate_impl(const Individual& individual) const override {
        int cut_size = 0;
        const auto& bit_vector = individual.bit_vector;
        for(int node = 0; node < individual.bit_vector.size(); ++node) {
            if(bit_vector[node] == BIT_ONE) {
                const auto& in_edges = instance.graph->getInEdges(node);
                const auto in_edges_size = in_edges.size();
                for(int edge = 0; edge < in_edges_size; ++edge) {
                    cut_size += in_edges[edge]->weight * (bit_vector[in_edges[edge]->start] == BIT_ZERO);
                }

                const auto& out_edges = instance.graph->getOutEdges(node);
                const auto out_edges_size = out_edges.size();
                for(int edge = 0; edge < out_edges_size; ++edge) {
                    cut_size += out_edges[edge]->weight * (bit_vector[out_edges[edge]->end] == BIT_ZERO);
                }
            }
        }
        return cut_size;
    }
};

class Maxdicut : public ObjectiveFunction {
public:
    Maxdicut(const Instance& p_instance) : ObjectiveFunction(p_instance) {}

    int evaluate_impl(const Individual& individual) const override {
        int cut_size = 0;
        const auto& bit_vector = individual.bit_vector;
        for(int node = 0; node < bit_vector.size(); ++node) {
            if(bit_vector[node] == BIT_ONE) {
                const auto& out_edges = instance.graph->getOutEdges(node);
                const auto out_edges_size = out_edges.size();
                for(int edge = 0; edge < out_edges_size; ++edge) {
                    cut_size += out_edges[edge]->weight * (bit_vector[out_edges[edge]->end] == BIT_ZERO);
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
