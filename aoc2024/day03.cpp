#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "tools/main.h"

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

MAIN("aoc2024/data/day03.txt", part1, part2);
