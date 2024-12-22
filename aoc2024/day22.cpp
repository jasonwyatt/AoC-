#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "tools/all.h"

unsigned long simulate(unsigned long secret, int days) {
    for (int i = 0; i < days; i++) {
        unsigned long shifted = secret << 6;
        secret = (shifted ^ secret) % 16777216;
        shifted = secret >> 5;
        secret = (shifted ^ secret) % 16777216;
        shifted = secret << 11;
        secret = (shifted ^ secret) % 16777216;
    }
    return secret;
}

void part1(std::ifstream& in) {
    unsigned long long total = 0;
    auto lambda = [&](std::string line) {
        total += simulate(stoul(line), 2000);
        return true;
    };
    tools::readLines(in, lambda);
    INFO("Part 1: %lld", total);
}

void add(std::vector<int>& seen, int change) {
    if (seen.size() == 4) {
        seen.erase(seen.begin());
    }
    seen.push_back(change);
}

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int val : v) {
            seed ^= hasher(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct VectorEqual {
    bool operator()(const std::vector<int>& a, const std::vector<int>& b) const {
        return a == b;
    }
};

struct Monkey {
    std::unordered_set<std::vector<int>, VectorHash, VectorEqual> sequences;
    std::unordered_map<std::vector<int>, int, VectorHash, VectorEqual> sequencePrices;
};

Monkey simulatePrices(unsigned long secret, int days) {
    int last;
    std::vector<int> seen;
    Monkey monkey;
    for (int i = 0; i < days; i++) {
        unsigned long shifted = secret << 6;
        secret = (shifted ^ secret) % 16777216;
        shifted = secret >> 5;
        secret = (shifted ^ secret) % 16777216;
        shifted = secret << 11;
        secret = (shifted ^ secret) % 16777216;

        int price = secret % 10;
        if (i > 0) {
            int change = price - last;
            add(seen, change);
            if (seen.size() == 4 && !monkey.sequences.contains(seen)) {
                monkey.sequencePrices[seen] = price;
                monkey.sequences.insert(seen);
            }
        }
        last = price;
    }
    return monkey;
}

bool isPossible(std::vector<int>& sequence, int index, int previousValue) {
    if (index == sequence.size()) return true;
    int diff = sequence[index];
    int next = diff + previousValue;
    if (next < 0 || next > 9) return false;
    return isPossible(sequence, index + 1, next);
}

bool isPossible(std::vector<int>& sequence) {
    for (int i = 0; i < 10; i++) {
        if (isPossible(sequence, 0, i)) return true;
    }
    return false;
}

std::vector<std::vector<int>> getSequences() {
    std::vector<std::vector<int>> sequences;
    for (int i = -9; i <= 9; i++) {
        for (int j = -9; j <= 9; j++) {
            for (int k = -9; k <= 9; k++) {
                for (int l = -9; l <= 9; l++) {
                    std::vector<int> sequence = {i, j, k, l};
                    if (isPossible(sequence)) {
                        sequences.push_back(sequence);
                    }
                }
            }
        }
    }
    return sequences;
}

void part2(std::ifstream& in) {
    std::vector<Monkey> monkeys;
    auto lambda = [&](std::string in) {
        monkeys.push_back(simulatePrices(std::stoul(in), 2000));
        return true;
    };
    tools::readLines(in, lambda);

    auto sequences = getSequences();
    int max = 0;
    for (auto& sequence : sequences) {
        int bought = 0;
        for (auto& monkey : monkeys) {
            if (monkey.sequences.contains(sequence)) {
                bought += monkey.sequencePrices[sequence];
            }
        }
        if (bought > max) {
            max = bought;
        }
    }
    INFO("Part 2: %d", max);
}

MAIN(part1, part2);