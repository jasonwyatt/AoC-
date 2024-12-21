#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

#include "tools/all.h"

using namespace tools::math;

Vec2i up(0, -1);
Vec2i down(0, 1);
Vec2i left(-1, 0);
Vec2i right(1, 0);
std::vector<std::pair<Vec2i, char>> dirs = {{ up, '^' } , { down, 'v' }, { left, '<' }, { right, '>' }};

std::map<std::tuple<char, char, int>, unsigned long> cache;

struct Robot {
    std::map<char, Vec2i>& coordsByChar;
    std::set<Vec2i> coords;

    bool isValid(const Vec2i& pos) {
        return coords.contains(pos);
    }

    std::vector<std::vector<char>> pathsBetween(char start, char end) {
        std::vector<std::vector<char>> result;
        pathsToValue(coordsByChar[end], coordsByChar[start], {}, result);
        return result;
    }

    void pathsToValue(Vec2i& dest, Vec2i pos, std::vector<char> soFar, std::vector<std::vector<char>>& options) {
        if (pos == dest) {
            auto nextSoFar = soFar;
            nextSoFar.push_back('A');
            options.push_back(nextSoFar);
            return;
        }
        auto diff = dest - pos;
        for (auto [dir, pathItem] : dirs) {
            auto candidate = pos + dir;
            if (!isValid(candidate)) continue;
            auto newDiff = dest - candidate;
            if (newDiff.manhattanMag() < diff.manhattanMag()) {
                auto nextSoFar = soFar;
                nextSoFar.push_back(pathItem);
                pathsToValue(dest, candidate, nextSoFar, options);
            }
        }
    }
};
Robot create(std::map<char, Vec2i>& coordsByChar) {
    std::set<Vec2i> coords;
    for (auto [c, coord] : coordsByChar) {
        coords.insert(coord);
    }
    return Robot {
        .coordsByChar = coordsByChar,
        .coords = coords
    };
}

// Position of the robot pressing a keypad.
// 7 8 9
// 4 5 6
// 1 2 3
//   0 A
std::map<char, Vec2i> keypadCoords = {
    { '7', Vec2i(0, 0) },
    { '8', Vec2i(1, 0) },
    { '9', Vec2i(2, 0) },
    { '4', Vec2i(0, 1) },
    { '5', Vec2i(1, 1) },
    { '6', Vec2i(2, 1) },
    { '1', Vec2i(0, 2) },
    { '2', Vec2i(1, 2) },
    { '3', Vec2i(2, 2) },
    { '0', Vec2i(1, 3) },
    { 'A', Vec2i(2, 3) },
};

// Position of a robot pressing a dirpad.
//   ^ A
// < v >
std::map<char, Vec2i> dirpadCoords = {
    { '^', Vec2i(1, 0) },
    { 'A', Vec2i(2, 0) },
    { '<', Vec2i(0, 1) },
    { 'v', Vec2i(1, 1) },
    { '>', Vec2i(2, 1) },
};

std::string stringify(std::vector<char> sequence) {
    std::ostringstream res;
    for (char c : sequence) {
        res << c;
    }
    return res.str();
}

unsigned long findBestSequenceLength(char start, char end, std::vector<Robot>& robots, int robotIndex) {
    std::tuple<char, char, int> key = {start, end, robotIndex};
    if (cache.contains(key)) return cache[key];
    if (robotIndex == robots.size() - 1) {
        auto allPaths = robots[robotIndex].pathsBetween(start, end);
        unsigned long min = -1;
        std::vector<char>& minPath = allPaths[0];
        for (auto path : allPaths) {
            if (path.size() < min) {
                minPath = path;
                min = path.size();
            }
        }
        cache[key] = min;
        return min;
    }
    auto allPaths = robots[robotIndex].pathsBetween(start, end);
    unsigned long minSum = -1;
    std::vector<char>& minPath = allPaths[0];
    for (auto path : allPaths) {
        char rStart = 'A';
        unsigned long sum = 0;
        for (char c : path) {
            sum += findBestSequenceLength(rStart, c, robots, robotIndex + 1);
            rStart = c;
        }
        if (sum < minSum) {
            minSum = sum;
            minPath = path;
        }
    }
    cache[key] = minSum;
    return minSum;
}

unsigned long score(std::string sequence, int dpadCount) {
    std::vector<Robot> robots = {
        create(keypadCoords),
    };
    for (int i = 0; i < dpadCount; i++) {
        robots.push_back(create(dirpadCoords));
    }

    unsigned long sum = 0;
    char start = 'A';
    for (char c : sequence) {
        sum += findBestSequenceLength(start, c, robots, 0);
        start = c;
    }
    return sum * std::stoi(sequence.substr(0, 3));
}

void part1(std::ifstream& in) {
    auto lines = tools::readLines(in);
    long sum = 0;
    for (auto line : lines) {
        long lscore = score(line, 2);
        sum += lscore;
    }
    INFO("Part 1: %ld", sum);
}

void part2(std::ifstream& in) {
    cache.clear();
    auto lines = tools::readLines(in);
    long sum = 0;
    for (auto line : lines) {
        long lscore = score(line, 25);
        sum += lscore;
    }
    INFO("Part 2: %ld", sum);
}

MAIN(part1, part2);