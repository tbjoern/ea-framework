#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

namespace eaframework {

enum {
    BIT_ONE = 1,
    BIT_ZERO = 0
};

struct Individual {
    std::vector<unsigned short> bit_vector;

    std::unordered_map<std::string, std::vector<double>> data_vectors;

    std::vector<double>& add_vector(std::string name);
};

}

std::ostream & operator<<(std::ostream &os, const eaframework::Individual& ind);
