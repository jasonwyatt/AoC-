#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include "absl/strings/str_split.h"
#include "tools/log.h"
#include "tools/main.h"

typedef unsigned long long ull;

bool solve(ull target, std::vector<ull>& terms, int index, ull soFar) {
    if (index == terms.size()) return target == soFar;
    if (soFar > target) return false;
    return solve(target, terms, index + 1, soFar + terms[index])
        || solve(target, terms, index + 1, soFar * terms[index]);
}


bool solve2(ull target, std::vector<ull>& terms, int index, ull soFar) {
    if (index == terms.size()) return target == soFar;
    if (soFar > target) return false;
    ull multiplier = 1;
    ull toShift = terms[index];
    while (toShift > 0) {
        toShift /= 10;
        multiplier *= 10;
    }
    return solve2(target, terms, index + 1, soFar + terms[index])
        || solve2(target, terms, index + 1, soFar * terms[index])
        || solve2(target, terms, index + 1, soFar * multiplier + terms[index]);
}

void part1(std::ifstream& in) {
    LOG_LEVEL(INFO_LEVEL);
    std::string line;
    ull result = 0;
    auto before = std::chrono::high_resolution_clock::now();
    while (std::getline(in, line)) {
        std::vector<std::string> colonParts = absl::StrSplit(line, ": ");
        ull target = std::stoull(colonParts[0]);

        std::vector<std::string> listStr = absl::StrSplit(colonParts[1], " ");
        std::vector<ull> terms;
        for (auto listItem : listStr) {
            terms.push_back(stoull(listItem));
        }

        if (solve(target, terms, 1, terms[0])) {
            DEBUG("%lld was found", target);
            result += target;
        }
    }
    auto after = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(after - before);
    INFO("Part 1: %lld, duration: %lld us", result, duration.count());
}

void part2(std::ifstream& in) {
    LOG_LEVEL(INFO_LEVEL);
    std::string line;
    ull result = 0;
    auto before = std::chrono::high_resolution_clock::now();
    while (std::getline(in, line)) {
        std::vector<std::string> colonParts = absl::StrSplit(line, ": ");
        ull target = std::stoull(colonParts[0]);

        std::vector<std::string> listStr = absl::StrSplit(colonParts[1], " ");
        std::vector<ull> terms;
        for (auto listItem : listStr) {
            terms.push_back(stoull(listItem));
        }

        if (solve2(target, terms, 1, terms[0])) {
            result += target;
        }
    }
    auto after = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(after - before);
    INFO("Part 2: %lld, duration: %lld us", result, duration.count());
}

MAIN(part1, part2);
