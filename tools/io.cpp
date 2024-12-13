#include "tools/io.h"

#include <fstream>
#include <vector>

tools::CharGrid tools::readCharGrid(std::ifstream &in) {
    std::string line;
    tools::CharGrid grid;
    while (std::getline(in, line)) {
        std::vector<char> chars;
        for (char c : line) {
            chars.push_back(c);
        }
        grid.push_back(chars);
    }
    return grid;
}

tools::IntGrid tools::readIntGrid(std::ifstream &in) {
    std::string line;
    tools::IntGrid grid;
    while (std::getline(in, line)) {
        std::vector<int> ints;
        for (char c : line) {
            ints.push_back(c - '0');
        }
        grid.push_back(ints);
    }
    return grid;
}
