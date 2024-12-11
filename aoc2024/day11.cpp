#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

#include "absl/strings/str_split.h"
#include "tools/log.h"
#include "tools/main.h"

typedef unsigned long long ull;

ull count(ull value, int stepsRemaining, std::map<std::pair<ull, int>, ull>& cache) {
    if (stepsRemaining == 0) {
        return 1;
    }
    std::pair<ull, int> key = { value, stepsRemaining };
    if (cache.contains(key)) return cache[key];

    ull result = 0;
    int digits = std::log10(value) + 1;
    if (value == 0) {
        result = count(1, stepsRemaining - 1, cache);
    } else if (digits % 2 == 0) {
        unsigned long divisor = 1;
        for (int i = 0; i < digits / 2; i++) {
            divisor *= 10;
        }
        result = count(value / divisor, stepsRemaining - 1, cache) 
                 + count(value % divisor, stepsRemaining - 1, cache);
    } else {
        result = count(value * 2024, stepsRemaining - 1, cache);
    }
    cache[key] = result;
    return result;
}

void part1(std::ifstream& in) {
    LOG_LEVEL(INFO_LEVEL);
    std::string line;
    std::getline(in, line);

    std::vector<std::string> parts = absl::StrSplit(line, " "); 
    std::map<std::pair<ull, int>, ull> cache;
    ull total = 0;
    for (std::string part : parts) {
        total += count(std::stoull(part), 25, cache);
    }
    INFO("Part 1: %lld", total);
}

void part2(std::ifstream& in) {
    LOG_LEVEL(INFO_LEVEL);
    std::string line;
    std::getline(in, line);

    std::vector<std::string> parts = absl::StrSplit(line, " "); 
    std::map<std::pair<ull, int>, ull> cache;
    ull total = 0;
    for (std::string part : parts) {
        total += count(std::stoull(part), 75, cache);
    }
    INFO("Part 2: %lld", total);
}

MAIN(part1, part2);