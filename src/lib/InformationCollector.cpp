#include "InformationCollector.hpp"
#include <EA.hpp>
#include <MutationOperator.hpp>
#include <ObjectiveFunction.hpp>
// #include <iostream>

namespace eaframework {

class DummyCollector : public InformationCollector {
    std::vector<double> results;
public:
    void generation_snapshot(const EA& ea) override {
        const auto& mutator = ea.getMutator();
        auto fitness = ea.getOffspringFitness();
        results.push_back(fitness);
    }

    void output_to_stream(std::ostream& stream) override {
        for(const auto& result: results) {
            stream << result << std::endl;
        }
    }
};

std::shared_ptr<InformationCollector> build_information_collector(InformationCollectorType) {
    return std::make_shared<DummyCollector>();
}

}
