#include <fstream>
#include <iostream>
#include <ranges>
#include <span>

#include "absl/strings/str_split.h"
#include "tools/main.h"

#define STOI_TRANSFORM [](std::string s) { return std::stoi(s); }

void part1(std::ifstream& file) {
    std::string line;
    std::vector<std::string> lineValues;

    size_t safeCount = 0;
    while (std::getline(file, line)) {
        lineValues = absl::StrSplit(line, " ");

        int32_t diff = 0;
        int32_t last = 0;
        size_t i = 0;
        bool isSafe = true;
        for (int32_t item : lineValues | std::views::transform(STOI_TRANSFORM)) {
            if (i > 0) {
                int32_t newDiff = item - last;
                uint32_t newMag = std::abs(newDiff);
                if (newMag < 1 || newMag > 3) {
                    isSafe = false;
                    break;
                }
                if ((diff < 0 && newDiff > 0) || (diff > 0 && newDiff < 0)) {
                    isSafe = false;
                    break;
                }
                diff = newDiff;
            }
            i++;
            last = item;
        }
        if (isSafe) {
            safeCount++;
        }
    }
    std::cout << "Part1: " << safeCount << std::endl;
}

bool isSafeDiff(int32_t lastDiff, int32_t currentDiff) {
    uint32_t currentMag = std::abs(currentDiff);
    if (currentMag < 1 || currentMag > 3) {
        return false;
    }
    if ((lastDiff < 0 && currentDiff > 0) || (lastDiff > 0 && currentDiff < 0)) {
        return false;
    }
    return true;
}

template<std::ranges::range T>
size_t isSafe(T sequence_, bool isFirst, bool dropped, int32_t lastDiff, int32_t last) {
    if (std::size(sequence_) == 0) return 1;
    auto sequence = std::ranges::subrange(sequence_);

    if (isFirst) {
        if (dropped) {
            return isSafe(sequence | std::views::drop(1),
                    /*isFirst=*/ false,
                    /*dropped=*/ dropped,
                    /*lastDiff=*/ 0,
                    /*last=*/sequence[0]);
        } else {
            size_t total = isSafe(sequence | std::views::drop(1),
                    /*isFirst=*/ true,
                    /*dropped=*/ true,
                    /*lastDiff=*/ 0,
                    /*last=*/0);
            total += isSafe(sequence | std::views::drop(1),
                    /*isFirst=*/false,
                    /*dropped=*/false,
                    /*lastDiff=*/0,
                    /*last=*/sequence[0]);
            return total > 0 ? 1 : 0;
        }
    } else {
        if (std::size(sequence) == 0) {
            return 1;
        }

        int32_t current = sequence[0];
        int32_t currentDiff = current - last;
        if (isSafeDiff(lastDiff, currentDiff)) {
            size_t total = isSafe(sequence | std::views::drop(1),
                    /*isFirst=*/false,
                    /*dropped=*/dropped,
                    /*lastDiff=*/currentDiff,
                    /*last=*/current);
            if (!dropped) {
                total += isSafe(sequence | std::views::drop(1),
                    /*isFirst=*/false,
                    /*dropped=*/true,
                    /*lastDiff=*/lastDiff,
                    /*last=*/last);
            }
            return total > 0 ? 1 : 0;
        } else if (!dropped) {
            return isSafe(sequence | std::views::drop(1),
                    /*isFirst=*/false,
                    /*dropped=*/true,
                    /*lastDiff=*/lastDiff,
                    /*last=*/last);
        } else {
            return 0;
        }
    }
}

void part2(std::ifstream& file) {
    std::string line;
    std::vector<std::string> lineValues;

    size_t safeCount = 0;
    while (std::getline(file, line)) {
        lineValues = absl::StrSplit(line, " ");
        auto x = lineValues | std::views::transform(STOI_TRANSFORM);
        auto x_ = std::ranges::subrange(x);
        safeCount += isSafe(x_, true, false, 0, 0);
    }
    std::cout << "Part2: " << safeCount << std::endl;
}

MAIN("aoc2024/data/day02.txt", part1, part2);
