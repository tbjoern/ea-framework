#include "InformationCollector.hpp"

namespace eaframework {

class DummyCollector : public InformationCollector {
public:
    void generation_snapshot(const EA&) override {

    }

    void output_to_stream(std::ostream&) override {
        
    }
};

std::shared_ptr<InformationCollector> build_information_collector(InformationCollectorType) {
    return std::make_shared<DummyCollector>();
}

}
