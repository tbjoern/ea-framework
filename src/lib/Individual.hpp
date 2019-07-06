#include <vector>
#include <unordered_map>
#include <string>

namespace eaframework {

enum {
    BIT_ONE = 1,
    BIT_ZERO = 0
};

struct Individual {
    std::vector<unsigned int> bit_vector;

    std::unordered_map<std::string, std::vector<double>> data_vectors;

    std::vector<double>& add_vector(std::string name);
};

}
