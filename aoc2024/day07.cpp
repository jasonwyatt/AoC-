#include <fstream>
#include <regex>
#include <string>
#include <iostream>
#include <vector>
#include <cstdint>

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
    return solve2(target, terms, index + 1, soFar + terms[index])
        || solve2(target, terms, index + 1, soFar * terms[index])
        || solve2(target, terms, index + 1, std::stoull(std::to_string(soFar) + std::to_string(terms[index])));
}

void part1(std::ifstream& in) {
    LOG_LEVEL(INFO_LEVEL);
    std::string line;
    ull result = 0;
    auto time = std::chrono::system_clock::now();
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
    auto duration = std::chrono::system_clock::now() - time;
    INFO("Part 1: %lld, duration: %ld", result, duration);
}

void part2(std::ifstream& in) {
    LOG_LEVEL(INFO_LEVEL);
    std::string line;
    ull result = 0;
    auto time = std::chrono::system_clock::now();
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
    auto duration = std::chrono::system_clock::now() - time;
    INFO("Part 2: %lld, duration: %d", result, duration);
}

MAIN(part1, part2);
