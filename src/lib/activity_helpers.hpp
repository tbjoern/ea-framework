#include <Individual.hpp>

namespace eaframework {

    class ObjectiveFunction;

    namespace activity {
        struct Parameters {
            int min, max, start, inc, dec;
            double decay_rate;

            Parameters() : min(-100), max(100), start(0), inc(1), dec(1), decay_rate(0.95) {}
        };

        typedef std::vector<std::vector<double>> Matrix;

        void init(const Parameters& params, Individual& individual);

        double activity_value(Individual& zero_individual, ObjectiveFunction& func, Bit bit_a, Bit bit_b);

        void update(const Parameters& activity_consts, Individual& individual, const std::vector<Bit>& bits, Individual& zero_individual, ObjectiveFunction& func);

        void decay(const Parameters& activity_consts, Individual& individual);
    }
}
