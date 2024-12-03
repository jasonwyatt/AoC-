#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>

void part1(std::ifstream& file) {
    std::regex pattern("mul\\(([\\d]{1,3}),([\\d]{1,3})\\)");
    std::string line;
    uint32_t result = 0;
    while (std::getline(file, line)) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), pattern);
        auto end = std::sregex_iterator();
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            result += stoi(match[1]) * stoi(match[2]);
        }
    }
    std::cout << "Part1: " << result << std::endl;
}

void part2(std::ifstream& file) {
    std::regex pattern("(do\\(\\))|(don't\\(\\))|mul\\(([\\d]{1,3}),([\\d]{1,3})\\)");
    std::string line;
    uint32_t result = 0;
    bool enabled = true;
    while (std::getline(file, line)) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), pattern);
        auto end = std::sregex_iterator();
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            if (enabled && match[3].length() > 0) {
                result += stoi(match[3]) * stoi(match[4]);
            } else if (match[1].length() > 0) {
                enabled = true;
            } else if (match[2].length() > 0) {
                enabled = false;
            }
        }
    }
    std::cout << "Part2: " << result << std::endl;
}

int main(int argc, char** argv) {
    const char* fileName;
    if (argc > 1) {
        fileName = argv[1];
    } else {
        fileName = "aoc2024/data/day03.txt";
    }

    {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Oops, file " << fileName << " cant be opened." << std::endl;
            return -1;
        }
        part1(file);
        file.close();
    }
    {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Oops, file " << fileName << " cant be opened." << std::endl;
            return -1;
        }
        part2(file);
        file.close();
    }
    return 0;
}
