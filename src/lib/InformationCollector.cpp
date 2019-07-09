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
    void generation_snapshot(int id, int run, int generation, const EA& ea) override {
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
        double time;
        int objectiveFunctionCalls;
        int generation;
    };
    std::unordered_map<int, std::vector<IterationData>> id_to_results;
public:
    void generation_snapshot(int id, int run, int generation, const EA& ea) override {
        int index = id * 1000 + run;
        if(id_to_results.count(index) == 0) {
            id_to_results[index] = std::vector<IterationData>();
        }
        auto& results = id_to_results[index];

        const auto& mutator = ea.getMutator();
        const auto& objective_function = ea.getObjectiveFunction();
        auto objective_function_calls = objective_function.callCount();
        auto fitness = ea.getOffspringFitness();
        auto time = ea.getMutationTime();
        results.push_back({fitness,time,objective_function_calls, generation});
    }

    void output_to_stream(std::ostream& stream) override {
        stream << "id, run, generation, fitness, time, objcalls" << std::endl;
        for(const auto& pair : id_to_results) {
            int id = pair.first / 1000;
            int run = pair.first % 1000;
            for(const auto& data : pair.second) {
                stream << id << "," << run << "," << data.generation << "," << data.fitness << "," << data.time << "," << data.objectiveFunctionCalls << std::endl;
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
