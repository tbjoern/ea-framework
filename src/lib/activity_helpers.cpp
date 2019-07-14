#include "activity_helpers.hpp"

#include <vector>
#include <ObjectiveFunction.hpp>

#include <iostream>

namespace eaframework {
namespace activity {
    void init(const Parameters& params, Individual& individual) {
        individual.add_vector("activity");
        auto& activity = individual.data_vectors["activity"];
        activity = std::vector<double>(individual.bit_vector.size(), params.start);
    }

    std::shared_ptr<Graph> activityValues(int bit_count, ObjectiveFunction& func) {
        auto zero_individual = Individual();
        zero_individual.bit_vector = std::vector<Bit>(bit_count, BIT_NONE);
        auto edges = std::vector<Edge>();
        for(int bit_a = 0; bit_a < bit_count; ++bit_a) {
            zero_individual.bit_vector[bit_a] = BIT_ONE;
            for(int bit_b = 0; bit_b < bit_count; ++bit_b) {
                if(bit_a == bit_b) {
                    continue;
                }
                zero_individual.bit_vector[bit_b] = BIT_ZERO;
                auto a_b = func.evaluate(zero_individual);
                if(a_b != 0) {
                    edges.push_back({bit_a, bit_b, a_b});
                }
                zero_individual.bit_vector[bit_b] = BIT_NONE;
            }
            zero_individual.bit_vector[bit_a] = BIT_NONE;
        }

        auto graph = std::make_shared<Graph>(bit_count, edges.size());
        for(const auto& edge : edges) {
            graph->addEdge(edge.start, edge.end, edge.weight);
        }

        return graph;
    }

    void update(const Parameters& activity_consts, Individual& individual, const std::vector<Bit>& bits, const Graph& graph) {
        auto& activity = individual.data_vectors["activity"];
        auto& bit_vector = individual.bit_vector;
        for (const auto bit : bits) {
            // use "outgoing edges" when in "cut set" (bit_one), use "incoming edges" when in "non cut set" (bit_zero)
            if(bit_vector[bit] == BIT_ONE) {
                const auto& in_edges = graph.getOutEdges(bit);
                for(const auto& edge : in_edges) {
                    auto bit_other = edge->end;
                    auto weight = edge->weight;
                    // increase when on the same side, decrease when on different sides
                    auto side_mod = (bit_vector[bit] == bit_vector[bit_other]) * activity_consts.inc - (bit_vector[bit] != bit_vector[bit_other]) * activity_consts.dec;
                    auto connection_mod = (bit_vector[bit] == BIT_ONE) * weight;
                    
                    activity[bit_other] += side_mod * connection_mod;
                }
            }
            else if (bit_vector[bit] == BIT_ZERO) {
                const auto& in_edges = graph.getInEdges(bit);
                for(const auto& edge : in_edges) {
                    auto bit_other = edge->start;
                    auto weight = edge->weight;
                    // increase when on the same side, decrease when on different sides
                    auto side_mod = (bit_vector[bit] == bit_vector[bit_other]) * activity_consts.inc - (bit_vector[bit] != bit_vector[bit_other]) * activity_consts.dec;
                    
                    auto connection_mod = (bit_vector[bit] == BIT_ZERO) * weight;
                    
                    activity[bit_other] += side_mod * connection_mod;
                }
            }
        }
        for (auto bit : bits) {
            activity[bit] = activity_consts.start;
        }
    }

    void decay(Parameters activity_consts, Individual& individual) {
        auto& activity = individual.data_vectors["activity"];
        auto& bit_vector = individual.bit_vector;
        for (auto& bit : bit_vector) {
            activity[bit] = (activity[bit] - activity_consts.start) * activity_consts.decay_rate + activity_consts.start;
        }
    }
}
}
