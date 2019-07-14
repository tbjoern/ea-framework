#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

namespace eaframework {

enum {
    BIT_ONE = 1,
    BIT_ZERO = 0,
    BIT_NONE = 2,
};

typedef unsigned short Bit;

struct Individual {
    std::vector<Bit> bit_vector;

    std::unordered_map<std::string, std::vector<double>> data_vectors;

    std::vector<double>& add_vector(std::string name);
};

}

std::ostream & operator<<(std::ostream &os, const eaframework::Individual& ind);
