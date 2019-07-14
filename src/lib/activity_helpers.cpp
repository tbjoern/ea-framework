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

    // takes too much memory -- for one graph this would take 270 GBs of RAM
    // std::vector<std::vector<double>> activityValues(Individual& individual, ObjectiveFunction& func) {
    //     const auto bit_count = individual.bit_vector.size();
    //     std::cout << bit_count * bit_count *sizeof(double) << std::endl;
    //     throw std::runtime_error("boom");
    //     auto matrix = std::vector<std::vector<double>>(bit_count, std::vector<double>(bit_count, 0));
    //     auto zero_individual = Individual();
    //     zero_individual.bit_vector = std::vector<Bit>(bit_count, BIT_NONE);
    //     for(int bit_a = 0; bit_a < bit_count; ++bit_a) {
    //         std::cout << bit_a << std::endl;
    //         zero_individual.bit_vector[bit_a] = BIT_ONE;
    //         for(int bit_b = 0; bit_b < bit_count; ++bit_b) {
    //             if(bit_a == bit_b) {
    //                 continue;
    //             }
    //             zero_individual.bit_vector[bit_b] = BIT_ZERO;
    //             auto a_b = func.evaluate(zero_individual);
    //             matrix[bit_a][bit_b] = a_b;
    //             zero_individual.bit_vector[bit_b] = BIT_NONE;
    //         }
    //         zero_individual.bit_vector[bit_a] = BIT_NONE;
    //     }

    //     return matrix;
    // }

    double activity_value(Individual& zero_individual, ObjectiveFunction& func, Bit bit_a, Bit bit_b) {
        if (bit_a == bit_b) {
            return 0;
        }
        std::cout << bit_a << "|" << bit_b << std::endl;
        zero_individual.bit_vector[bit_a] = BIT_ONE;
        zero_individual.bit_vector[bit_b] = BIT_ZERO;
        auto a_b = func.evaluate(zero_individual);
        zero_individual.bit_vector[bit_a] = BIT_NONE;
        zero_individual.bit_vector[bit_b] = BIT_NONE;
        return a_b;
    }

    void update(const Parameters& activity_consts, Individual& individual, const std::vector<Bit>& bits, Individual& zero_individual, ObjectiveFunction& func) {
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
                if(bit_vector[bit] == BIT_ONE) {
                    activity[bit_other] += side_mod * activity_value(zero_individual, func, bit, bit_other);
                }
                else if (bit_vector[bit] == BIT_ZERO) {
                    activity[bit_other] += side_mod * activity_value(zero_individual, func, bit_other, bit);
                }
            }
        }
        for (auto bit : bits) {
            activity[bit] = activity_consts.start;
        }
    }

    void decay(const Parameters& activity_consts, Individual& individual) {
        auto& activity = individual.data_vectors["activity"];
        auto& bit_vector = individual.bit_vector;
        for (auto& bit : bit_vector) {
            activity[bit] = (activity[bit] - activity_consts.start) * activity_consts.decay_rate + activity_consts.start;
        }
    }
}
}
