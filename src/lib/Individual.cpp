#include "Individual.hpp"

#include <exception>

namespace eaframework {

std::vector<double>& Individual::add_vector(std::string name) {
    if(data_vectors.count(name)) {
        throw std::invalid_argument("Vector with key " + name + " already exists");
    }
    data_vectors[name] = std::vector<double>(bit_vector.size());
    return data_vectors[name];
}


}

std::ostream & operator<<(std::ostream &os, const eaframework::Individual& ind)
{
    os << ind.bit_vector.size() << std::endl;
    for(int bit = 0; bit < ind.bit_vector.size(); ++bit) {
        if(ind.bit_vector[bit] == eaframework::BIT_ONE) {
            os << bit << std::endl;
        }
    }
    return os;
}
