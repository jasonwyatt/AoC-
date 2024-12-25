#include <fstream>
#include <map>
#include <vector>
#include <set>

#include "tools/all.h"

enum Type {
    KEY, LOCK
};

std::vector<std::pair<Type, std::vector<int>>> read(std::ifstream& in) {
    std::vector<std::string> lines;
    std::vector<std::pair<Type, std::vector<int>>> result;
    auto finish = [&]() {
        std::vector<int> heights;
        Type type;
        if (lines[0][0] == '#') {
            // Lock
            type = LOCK;
            for (int i = 0; i < lines[0].size(); i++) {
                for (int j = 1; j < lines.size(); j++) {
                    if (lines[j][i] != '#')  {
                        heights.push_back(j - 1);
                        break;
                    }
                }
            }
        } else {
            type = KEY;
            int height = lines.size() - 2;
            for (int i = 0; i < lines[0].size(); i++) {
                int k = 0;
                for (int j = height; j >= 0; j--) {
                    if (lines[j][i] != '#')  {
                        heights.push_back(k);
                        break;
                    }
                    k++;
                }
            }
        }
        lines.clear();
        result.push_back({type, heights});
    };
    auto lambda = [&](std::string line) {
        if (line.size() == 0) {
            finish();
        } else {
            lines.push_back(line);
        }
        return true;
    };
    tools::readLines(in, lambda);
    finish();
    return result;
}

void part1(std::ifstream& in) {
    auto parts = read(in);
    std::vector<std::vector<int>> keys;
    std::vector<std::vector<int>> locks;
    for (auto& [type, part] : parts) {
        if (type == KEY) {
            keys.push_back(part);
        } else {
            locks.push_back(part);
        }
    }

    int count = 0;
    for (auto& lock : locks) {
        for (auto& key : keys) {
            bool isGood = true;
            for (int i = 0; i < lock.size(); i++) {
                if (lock[i] + key[i] > 5) {
                    isGood = false;
                    break;
                }
            }
            if (isGood) count++;
        }
    }
    INFO("Part 1: %d", count);
}

void part2(std::ifstream& in) {

}

MAIN(part1, part2);