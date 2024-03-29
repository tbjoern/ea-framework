#pragma once
#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>

namespace eaframework {

class EA;
enum class InformationCollectorType {
    DEFAULT,
    ITERATIONDATA,
};


class InformationCollector {
public:
    virtual void write_header(std::ostream&) = 0;

    virtual void generation_snapshot(int id, int run, int generation, const EA&) = 0;

    virtual void output_to_stream(std::ostream&) = 0;

    virtual void clear() = 0;
};

// we only need one information collector, but we want to make use of polymorphism
// thus we provide a factory method
std::shared_ptr<InformationCollector> build_information_collector(InformationCollectorType);

}
