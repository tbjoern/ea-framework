#pragma once
#include <string>
#include <fstream>

namespace eaframework {

inline std::pair<std::string, std::string> split_filename(std::string filename) {
    auto dot_pos = filename.rfind('.');
    return {filename.substr(0, dot_pos), filename.substr(dot_pos)};
}

inline bool file_exists(std::string filename) {
    std::ifstream filestream(filename);
    return filestream.good();
}

}
