#include <fstream>
#include <set>
#include <queue>
#include <vector>

#include "tools/log.h"
#include "tools/main.h"

typedef std::vector<std::vector<int>> TopoMap;

int countSummits(TopoMap& map, int i, int j) {
    std::set<std::pair<int, int>> visited;
    std::queue<std::pair<int, int>> queue;

    queue.push({i, j});
    visited.insert({i, j});

    int height = map.size();
    int width = map[0].size();

    int summits = 0;
    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        int currentAlt = map[current.first][current.second];
        if (currentAlt == 9) {
            summits++;
            continue;
        }

        std::pair<int, int> up = { current.first - 1, current.second };
        std::pair<int, int> down = { current.first + 1, current.second };
        std::pair<int, int> left = { current.first, current.second - 1 };
        std::pair<int, int> right = { current.first, current.second + 1 };
        if (up.first >= 0) {
            if (map[up.first][up.second] == currentAlt + 1 && !visited.contains(up)) {
                visited.insert(up);
                queue.push(up);
            }
        }
        if (down.first < height) {
            if (map[down.first][down.second] == currentAlt + 1 && !visited.contains(down)) {
                visited.insert(down);
                queue.push(down);
            }
        }
        if (left.second >= 0) {
            if (map[left.first][left.second] == currentAlt + 1 && !visited.contains(left)) {
                visited.insert(left);
                queue.push(left);
            }
        }
        if (right.second < width) {
            if (map[right.first][right.second] == currentAlt + 1 && !visited.contains(right)) {
                visited.insert(right);
                queue.push(right);
            }
        }
    }
    return summits;
}

int countTrails(TopoMap& map, int i, int j) {
    if (map[i][j] == 9) return 1;

    int total = 0;
    if (i - 1 >= 0 && map[i - 1][j] == map[i][j] + 1) {
        total += countTrails(map, i - 1, j);
    }
    if (i + 1 < map.size() && map[i + 1][j] == map[i][j] + 1) {
        total += countTrails(map, i + 1, j);
    }
    if (j - 1 >= 0 && map[i][j - 1] == map[i][j] + 1) {
        total += countTrails(map, i, j - 1);
    }
    if (j + 1 < map[0].size() && map[i][j + 1] == map[i][j] + 1) {
        total += countTrails(map, i, j + 1);
    }
    return total;
}

void part1(std::ifstream& in) {
    TopoMap map;
    std::string line;
    while (std::getline(in, line)) {
        std::vector<int> row;
        for (char c : line) {
            row.push_back(c - '0');
        }
        map.push_back(row);
    }

    int total = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == 0) {
                total += countSummits(map, i, j);
            }
        }
    }
    INFO("Part 1: %d", total);
}

void part2(std::ifstream& in) {
    TopoMap map;
    std::string line;
    while (std::getline(in, line)) {
        std::vector<int> row;
        for (char c : line) {
            row.push_back(c - '0');
        }
        map.push_back(row);
    }

    int total = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == 0) {
                total += countTrails(map, i, j);
            }
        }
    }
    INFO("Part 2: %d", total);
}

MAIN(part1, part2);