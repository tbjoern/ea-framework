#include <memory>
#include <iostream>

namespace eaframework {

class EA;
enum class InformationCollectorType {
    DEFAULT
};

class InformationCollector {
public:
    virtual void generation_snapshot(const EA&) = 0;

    virtual void output_to_stream(std::ostream&) = 0;
};

// we only need one information collector, but we want to make use of polymorphism
// thus we provide a factory method
std::shared_ptr<InformationCollector> make_information_collector(InformationCollectorType);

}
