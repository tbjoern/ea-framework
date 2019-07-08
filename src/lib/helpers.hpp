#include <string>

namespace eaframework {

std::string get_extension(std::string filename) {
    return filename.substr(filename.rfind('.'));
}

}
