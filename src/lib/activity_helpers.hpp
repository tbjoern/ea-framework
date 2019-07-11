#include <Individual.hpp>

namespace eaframework {

    class ObjectiveFunction;

    namespace activity {
        struct Parameters {
            int min, max, start, inc, dec;
            double decay_rate;
        };

        void init(Parameters params, Individual& individual);

        std::vector<std::vector<double>> activityValues(Individual& individual, ObjectiveFunction& func);

        void update(Parameters activity_consts, Individual& individual, std::vector<Bit> bits, std::vector<std::vector<double>> matrix);

        void decay(Parameters activity_consts, Individual& individual);
    }
}
