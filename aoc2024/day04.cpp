#include <fstream>
#include <iostream>
#include <vector>

#include "tools/log.h"
#include "tools/main.h"

const char* target = "XMAS";

int check(std::vector<std::string>& lines, int i, int j) {
    int count = 0;
    // forward vertical
    if (i + strlen(target) <= lines.size()) {
        bool keepGoing = true;
        for (int k = 0; k < strlen(target); k++) {
            if (target[k] != lines[i + k][j]) {
                keepGoing = false;
            }
            if (!keepGoing) break;
        }
        if (keepGoing) {
            count++;
        }
    }
    // backward vertical
    if (i - strlen(target) + 1 >= 0) {
        bool keepGoing = true;
        for (int k = 0; k < strlen(target); k++) {
            if (target[k] != lines[i - k][j]) {
                keepGoing = false;
            }
            if (!keepGoing) break;
        }
        if (keepGoing) {
            count++;
        }
    }
    // forward horizontal
    if (j + strlen(target) <= lines[0].size()) {
        bool keepGoing = true;

        for (int k = 0; k < strlen(target); k++) {
            if (target[k] != lines[i][j + k]) {
                keepGoing = false;
            }
            if (!keepGoing) break;
        }
        if (keepGoing) {
            count++;
        }
    }
    // backward horizontal
    if (j - strlen(target) + 1 >= 0) {
        bool keepGoing = true;
        for (int k = 0; k < strlen(target); k++) {
            if (target[k] != lines[i][j - k]) {
                keepGoing = false;
            }
            if (!keepGoing) break;
        }
        if (keepGoing) {
            count++;
        }
    }
    // forward diagonal 1
    if (i + strlen(target) <= lines.size() && j + strlen(target) <= lines[0].size()) {
        bool keepGoing = true;

        for (int k = 0; k < strlen(target); k++) {
            if (target[k] != lines[i + k][j + k]) {
                keepGoing = false;
            }
            if (!keepGoing) break;
        }
        if (keepGoing) {
            count++;
        }
    }
    // forward diagonal 2
    if (i + strlen(target) <= lines.size() && j - strlen(target) + 1 >= 0) {
        bool keepGoing = true;

        for (int k = 0; k < strlen(target); k++) {
            if (target[k] != lines[i + k][j - k]) {
                keepGoing = false;
            }
            if (!keepGoing) break;
        }
        if (keepGoing) {
            count++;
        }
    }
    // backward diagonal-1
    if (i - strlen(target) + 1 >= 0 && j - strlen(target) + 1 >= 0) {
        bool keepGoing = true;
        for (int k = 0; k < strlen(target); k++) {
            if (target[k] != lines[i - k][j - k]) {
                keepGoing = false;
            }
            if (!keepGoing) break;
        }
        if (keepGoing) {
            count++;
        }
    }
    // backward diagonal-2
    if (i - strlen(target) + 1 >= 0 && j + strlen(target) <= lines[0].size()) {
        bool keepGoing = true;
        for (int k = 0; k < strlen(target); k++) {
            if (target[k] != lines[i - k][j + k]) {
                keepGoing = false;
            }
            if (!keepGoing) break;
        }
        if (keepGoing) {
            count++;
        }
    }
    return count;
}

void part1(std::ifstream& file) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(std::string(line));
    }

    int total = 0;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines.size(); j++) {
            total += check(lines, i, j);
        }
    }

    std::cout << "Part1: " << total << std::endl;
}

void part2(std::ifstream& file) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(std::string(line));
    }

    std::vector<std::vector<std::string>> masks = {
        {
            "M.S",
            ".A.",
            "M.S",
        },
        {
            "S.S",
            ".A.",
            "M.M",
        },
        {
            "M.M",
            ".A.",
            "S.S",
        },
        {
            "S.M",
            ".A.",
            "S.M",
        },
    };

    int count = 0;

    for (int i = 0; i + 2 < lines.size(); i++) {
        for (int j = 0; j + 2 < lines[0].size(); j++) {
            for (auto mask : masks) {
                bool valid = true;
                for (int u = 0; u < 3; u++) {
                    for (int v = 0; v < 3; v++) {
                        if (mask[u][v] == '.') continue;
                        if (mask[u][v] != lines[i+u][j+v]) {
                            valid = false;
                        }
                    }
                }
                if (valid) count++;
            }
        }
    }

    std::cout << "Part2: " << count << std::endl;
}

MAIN(part1, part2);