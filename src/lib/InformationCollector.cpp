#include "InformationCollector.hpp"
#include <EA.hpp>
#include <MutationOperator.hpp>
#include <ObjectiveFunction.hpp>

#include <vector>
#include <unordered_map>

namespace eaframework {

class DummyCollector : public InformationCollector {
    std::vector<double> results;
public:
    void write_header(std::ostream& stream) override {
       
    }
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

    void clear() override {
        results.clear();
    }
};

class IterationDataCollector : public InformationCollector {
    struct IterationData {
        int fitness;
        double time;
        int objectiveFunctionCalls;
        int generation;
        int flips;
        bool generationImproved;
    };
    std::unordered_map<int, std::vector<IterationData>> id_to_results;
    std::unordered_map<int, IterationData> id_to_last_result;
public:
    void write_header(std::ostream& stream) override {
        stream << "id, run, generation, fitness, mutation_time, objcalls, flips, improved" << std::endl;
    }

    bool record_generation(int generation) {
        if( generation < 100 )
        {
            return true;
        } else if ( generation < 1000 && generation % 10 == 0 )
        {
            return true;
        } else if ( generation % 100 == 0 )
        {
            return true;
        }
        return false;
    }

    void generation_snapshot(int id, int run, int generation, const EA& ea) override {
        if(!record_generation(generation)) {
            return;
        }
        int index = id * 1000 + run;
        if(id_to_results.count(index) == 0) {
            id_to_results[index] = std::vector<IterationData>();
        }
        if(id_to_last_result.count(index) == 0) {
            id_to_last_result[index] = IterationData();
        }
        auto& results = id_to_results[index];

        const auto& objective_function = ea.getObjectiveFunction();
        auto objective_function_calls = objective_function.callCount();
        auto fitness = ea.getOffspringFitness();
        auto time = ea.getMutationTime();
        auto flips = ea.getMutator().get_flip_count();
        auto improved = ea.generationImproved();
        results.push_back({fitness,time,objective_function_calls, generation, flips, improved});
    }

    void output_to_stream(std::ostream& stream) override {
        for(const auto& pair : id_to_results) {
            int id = pair.first / 1000;
            int run = pair.first % 1000;
            auto& last_result = id_to_last_result[pair.first];
            for(const auto& data : pair.second) {
                last_result.generation = data.generation;
                last_result.fitness = std::max(last_result.fitness, data.fitness);
                last_result.time += data.time;
                last_result.objectiveFunctionCalls = data.objectiveFunctionCalls;
                last_result.flips = data.flips;
                last_result.generationImproved = data.generationImproved;
                stream << id << "," << run << "," << last_result.generation << "," << last_result.fitness << "," << last_result.time << "," << last_result.objectiveFunctionCalls << "," << last_result.flips << "," << last_result.generationImproved << std::endl;
            }
        }
    }

    void clear() override {
        id_to_results.clear();
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
