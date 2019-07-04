#include <vector>
#include <unordered_map>
#include <string>

namespace eaframework {

struct Individual {
    std::vector<unsigned int> bit_vector;

    std::unordered_map<std::string, std::vector<double>> data_vectors;

    std::vector<double>& add_vector(std::string name);
};

}
