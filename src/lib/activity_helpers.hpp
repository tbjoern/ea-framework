#include <Individual.hpp>
#include <Graph.hpp>
#include <memory>

namespace eaframework {

    class ObjectiveFunction;

    namespace activity {
        struct Parameters {
            int min, max, start, inc, dec;
            double decay_rate;

            Parameters() : min(-100), max(100), start(0), inc(1), dec(1), decay_rate(0.95) {}
        };

        void init(const Parameters& params, Individual& individual);

        std::shared_ptr<Graph> activityValues(int bit_count, ObjectiveFunction& func);

        void update(const Parameters& activity_consts, Individual& individual, const std::vector<Bit>& bits, const Graph& graph);

        void decay(Parameters activity_consts, Individual& individual);
    }
}
