#include "activity_helpers.hpp"

#include <vector>
#include <ObjectiveFunction.hpp>

namespace eaframework {
namespace activity {
    void init(Parameters params, Individual& individual) {
        individual.add_vector("activity");
        auto& activity = individual.data_vectors["activity"];
        activity = std::vector<double>(individual.bit_vector.size(), params.start);
    }

    std::vector<std::vector<double>> activityValues(Individual& individual, ObjectiveFunction& func) {
        const auto bit_count = individual.bit_vector.size();
        auto matrix = std::vector<std::vector<double>>(bit_count, std::vector<double>(bit_count, 0));
        auto zero_individual = Individual();
        zero_individual.bit_vector = std::vector<Bit>(bit_count, BIT_NONE);
        for(int bit_a = 0; bit_a < bit_count; ++bit_a) {
            zero_individual.bit_vector[bit_a] = BIT_ONE;
            for(int bit_b = 0; bit_b < bit_count; ++bit_b) {
                if(bit_a == bit_b) {
                    continue;
                }
                zero_individual.bit_vector[bit_b] = BIT_ZERO;
                auto a_b = func.evaluate(zero_individual);
                matrix[bit_a][bit_b] = a_b;
                zero_individual.bit_vector[bit_b] = BIT_NONE;
            }
            zero_individual.bit_vector[bit_a] = BIT_NONE;
        }

        return matrix;
    }

    void update(Parameters activity_consts, Individual& individual, std::vector<Bit> bits, Matrix matrix) {
        auto& activity = individual.data_vectors["activity"];
        auto& bit_vector = individual.bit_vector;
        for (const auto bit : bits) {
            for(int bit_other = 0; bit_other < bit_vector.size(); ++bit_other) {
                if (bit == bit_other) {
                    continue;
                }

                // increase when on the same side, decrease when on different sides
                auto side_mod = (bit_vector[bit] == bit_vector[bit_other]) * activity_consts.inc - (bit_vector[bit] != bit_vector[bit_other]) * activity_consts.dec;

                // use "outgoing edges" when in "cut set" (bit_one), use "incoming edges" when in "non cut set" (bit_zero)
                auto connection_mod = (bit_vector[bit] == BIT_ONE) * matrix[bit][bit_other] + (bit_vector[bit] == BIT_ZERO) * matrix[bit_other][bit];
                
                activity[bit_other] += side_mod * connection_mod;
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
