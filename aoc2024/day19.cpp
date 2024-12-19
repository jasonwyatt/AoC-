#include <vector>
#include <fstream>

#include "tools/all.h"

long countWays(const std::string& target, const std::vector<std::string>& options) {
    std::vector<long> ways = std::vector<long>(target.size() + 1, 0);
    ways[0] = 1;
    for (int i = 1; i < ways.size(); i++) {
        for (auto option : options) {
            if (i < option.size()) continue;
            if (target.substr(i - option.size(), option.size()) == option && ways[i - option.size()] > 0) {
                ways[i] += ways[i - option.size()];
            }
        }
    }
    return ways[target.size()];
}

void part1(std::ifstream& in) {
    std::string line;
    std::getline(in, line);

    auto options = tools::split(line, ", ");

    // blank line
    std::getline(in, line);

    auto targets = tools::readLines(in);

    int count = 0;
    for (auto target : targets) {
        long fastWays = countWays(target, options);
        if (fastWays > 0) {
            count++;
        }
    }
    INFO("Part 1: %d", count);
}

void part2(std::ifstream& in) {
    std::string line;
    std::getline(in, line);

    auto options = tools::split(line, ", ");

    // blank line
    std::getline(in, line);

    auto targets = tools::readLines(in);

    unsigned long long count = 0;
    for (auto target : targets) {
        long fastWays = countWays(target, options);
        count += fastWays;
    }
    INFO("Part 2: %lld", count);
}

MAIN(part1, part2);