#include "InformationCollector.hpp"
#include <EA.hpp>
#include <MutationOperator.hpp>
#include <ObjectiveFunction.hpp>

#include <vector>
#include <unordered_map>
// #include <iostream>

namespace eaframework {

class DummyCollector : public InformationCollector {
    std::vector<double> results;
public:
    void generation_snapshot(int id, int run, const EA& ea) override {
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

class IterationDataCollector : public InformationCollector {
    struct IterationData {
        double fitness;
        int run;
    };
    std::unordered_map<int, std::vector<IterationData>> id_to_results;
public:
    void generation_snapshot(int id, int run, const EA& ea) override {
        if(id_to_results.count(id) == 0) {
            id_to_results[id] = std::vector<IterationData>();
        }
        auto& results = id_to_results[id];

        const auto& mutator = ea.getMutator();
        auto fitness = ea.getOffspringFitness();
        results.push_back({fitness,run});
    }

    void output_to_stream(std::ostream& stream) override {
        for(const auto& pair : id_to_results) {
            int id = pair.first;
            for(const auto& data : pair.second) {
                stream << id << "," << data.run << "," << data.fitness << std::endl;
            }
        }
    }
};

std::shared_ptr<InformationCollector> build_information_collector(InformationCollectorType type) {
    switch(type) {
        case InformationCollectorType::DEFAULT:
            return std::make_shared<DummyCollector>();
        case InformationCollectorType::ITERATIONDATA:
            return std::make_shared<IterationDataCollector>();
        default:
            throw std::invalid_argument("Unknown InformationCollector type");
    }
}

}
