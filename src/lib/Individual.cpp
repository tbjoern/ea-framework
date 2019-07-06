#include "Individual.hpp"

#include <exception>

namespace eaframework {

std::vector<double>& Individual::add_vector(std::string name) {
    if(data_vectors.count[name]) {
        throw std::invalid_argument("Vector with key " + name + " already exists");
    }
    data_vectors[name] = std::vector<double>(bit_vector.size());
    return data_vectors[name];
}

}
