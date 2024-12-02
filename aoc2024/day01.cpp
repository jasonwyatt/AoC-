#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <vector>
#include "absl/container/flat_hash_map.h"
#include "absl/strings/str_split.h"

using namespace std;

void part1(const char* fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Oops, file " << fileName << " cant be opened." << endl;
        return;
    }

    vector<int> left;
    vector<int> right;
    string line;
    while (getline(file, line)) {
        vector<string> parts = absl::StrSplit(line, "   ");
        auto leftVal = stod(parts[0]);
        auto lefti = lower_bound(left.begin(), left.end(), leftVal);
        left.insert(lefti, leftVal);
        auto rightVal = stod(parts[1]);
        auto righti = lower_bound(right.begin(), right.end(), rightVal);
        right.insert(righti, rightVal);
    }

    file.close();

    size_t len = left.size();
    long result = 0;
    for (size_t i = 0; i < len; i++) {
        result += abs(left[i] - right[i]);
    }

    cout << "Part1: " << result  << endl;
}

void part2(const char* fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Oops, file " << fileName << " cant be opened." << endl;
        return;
    }

    vector<int> left;
    absl::flat_hash_map<int, int> rightCounts;
    string line;
    while (getline(file, line)) {
        vector<string> parts = absl::StrSplit(line, "   ");
        left.push_back(stoi(parts[0]));

        int rightVal = stoi(parts[1]);
        if (rightCounts.contains(rightVal)) {
            rightCounts[rightVal] = rightCounts[rightVal] + 1;
        } else {
            rightCounts[rightVal] = 1;
        }
    }
    file.close();

    long result = 0;
    size_t len = left.size();
    for (size_t i = 0; i < len; i++) {
        int leftValue = left[i];
        if (rightCounts.contains(leftValue)) {
            result += leftValue * rightCounts[leftValue];
        }
    }
    cout << "Part2: " << result << endl;
}

int main(int argc, const char** argv) {
    const char* fileName;
    if (argc > 1) {
        fileName = argv[1];
    } else {
        fileName = "aoc2024/data/day01.txt";
    }

    part1(fileName);
    part2(fileName);
    return 0;
}
