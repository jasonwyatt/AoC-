#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <queue>

#include "tools/all.h"

using namespace tools::math;

Vec2<int> dest(70, 70);
int maxRead = 1024;

struct Grid {
    std::set<Vec2<int>> corrupted;
    int width;
    int height;

    std::string str(std::vector<Vec2<int>>& path) {
        std::set<Vec2<int>> pathSet;
        for (auto p : path) {
            pathSet.insert(p);
        }
        std::ostringstream result;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Vec2<int> point(x, y);
                if (corrupted.contains(point)) {
                    result << "#";
                } else if (pathSet.contains(point)) {
                    result << "O";
                } else {
                    result << ".";
                }
            }
            result << std::endl;
        }
        return result.str();
    }
};

Grid parse(std::ifstream& in, int width, int height, int max) {
    Grid result;
    result.width = width;
    result.height = height;

    int i = 0;
    auto lambda = [&](std::string line) {
        auto parts = tools::split(line, ",");
        result.corrupted.insert(Vec2<int>(stoi(parts[0]), stoi(parts[1])));
        i++;
        return i < max;
    };
    tools::readLines(in, lambda);

    return result;
}

typedef std::vector<Vec2<int>> Path;
typedef std::pair<Path, int> QueueEntry;

bool operator<(const QueueEntry& a, const QueueEntry& b) {
    auto aDist = (a.first.back() - dest).mag();
    auto bDist = (b.first.back() - dest).mag();
    if (aDist == bDist) {
        return a.second < b.second;
    }
    return aDist < bDist;
}

bool hasPath(Grid& grid, std::set<Vec2<int>>& soFar, Vec2<int> last) {
    if (last.i == grid.width - 1 && last.j == grid.height - 1) return true;

    std::vector<Vec2<int>> options = {
        last + Vec2<int>(0, -1),
        last + Vec2<int>(0, 1),
        last + Vec2<int>(1, 0),
        last + Vec2<int>(-1, 0)
    };
    for (auto option : options) {
        if (option.i >= grid.width || option.i < 0 || option.j >= grid.height || option.j < 0) continue;
        if (soFar.contains(option)) continue;
        if (grid.corrupted.contains(option)) continue;
        soFar.insert(option);
        if (hasPath(grid, soFar, option)) {
            return true;
        }
        soFar.erase(option);
    }
    return false;
}

bool operator<(Vec2<int>& lhs, Vec2<int>& rhs) {
    return lhs.mag() < rhs.mag();
}

bool hasPath(Grid& grid) {
    Vec2<int> start(0, 0);
    std::set<Vec2<int>> visited = { start };
    std::priority_queue<Vec2<int>> queue;
    queue.push(start);

    while (!queue.empty()) {
        auto item = queue.top();
        if (item.i == grid.width - 1 && item.j == grid.height - 1) return true;
        queue.pop();

        std::vector<Vec2<int>> options = {
            item + Vec2<int>(0, -1),
            item + Vec2<int>(0, 1),
            item + Vec2<int>(1, 0),
            item + Vec2<int>(-1, 0)
        };
        for (auto option : options) {
            if (option.i >= grid.width || option.i < 0 || option.j >= grid.height || option.j < 0) continue;
            if (visited.contains(option)) continue;
            if (grid.corrupted.contains(option)) continue;
            visited.insert(option);
            queue.push(option);
        }
    }
    return false;
}

std::vector<Path> getPaths(Grid& grid) {
    std::vector<Path> result;

    std::priority_queue<QueueEntry> queue;
    queue.push({{Vec2<int>(0, 0)}, 0});
    std::map<Vec2<int>, int> visited;
    visited[Vec2<int>(0, 0)] = 0;

    while (!queue.empty()) {
        auto [positions, steps] = queue.top();
        queue.pop();

        if (positions.back().i == grid.width - 1 && positions.back().j == grid.height - 1) {
            result.push_back(positions);
            continue;
        }
        auto up = positions.back() + Vec2<int>(0, -1);
        int existingUpScore = visited[up];
        auto down = positions.back() + Vec2<int>(0, 1);
        int existingDownScore = visited[down];
        auto right = positions.back() + Vec2<int>(1, 0);
        int existingRightScore = visited[right];
        auto left = positions.back() + Vec2<int>(-1, 0);
        int existingLeftScore = visited[left];
        if (up.j < grid.height && up.j >= 0 && !grid.corrupted.contains(up) && (existingUpScore == 0 || existingUpScore > steps + 1)) {
            std::vector<Vec2<int>> nextPos = positions;
            nextPos.push_back(up);
            queue.push({nextPos, steps + 1});
            visited[up] = steps + 1;
        }
        if (down.j < grid.height && down.j >= 0 && !grid.corrupted.contains(down) && (existingDownScore == 0 || existingDownScore > steps + 1)) {
            std::vector<Vec2<int>> nextPos = positions;
            nextPos.push_back(down);
            queue.push({nextPos, steps + 1});
            visited[down] = steps + 1;
        }
        if (right.i < grid.width && right.i >= 0 && !grid.corrupted.contains(right) && (existingRightScore == 0 || existingRightScore > steps + 1)) {
            std::vector<Vec2<int>> nextPos = positions;
            nextPos.push_back(right);
            queue.push({nextPos, steps + 1});
            visited[right] = steps + 1;
        }
        if (left.i < grid.width && left.i >= 0 && !grid.corrupted.contains(left) && (existingLeftScore == 0 || existingLeftScore > steps + 1)) {
            std::vector<Vec2<int>> nextPos = positions;
            nextPos.push_back(left);
            queue.push({nextPos, steps + 1});
            visited[left] = steps + 1;
        }
    }

    return result;
}

void part1(std::ifstream& in) {
    Grid grid = parse(in, dest.i + 1, dest.j + 1, maxRead);

    int min = INT_MAX;
    std::vector<Vec2i>* bestPath;
    auto paths = getPaths(grid);
    for (auto path : paths) {
        if (path.size() < min) {
            min = path.size();
            bestPath = &path;
        }
    }

    INFO("Part 1: %d", min);
}

void part2(std::ifstream& in) {
    Grid grid{
        .width = dest.i + 1,
        .height = dest.j + 1,
    };

    auto lambda = [](std::string line) {
        auto parts = tools::split(line, ",");
        return Vec2<int>(stoi(parts[0]), stoi(parts[1]));
    };
    std::vector<Vec2<int>> bytes = tools::interpretLines<Vec2<int>>(in, lambda);

    for (int i = 0; i < bytes.size(); i++) {
        std::vector<Vec2<int>> testItems(bytes.begin(), bytes.begin() + i);
        grid.corrupted = std::set<Vec2<int>>(testItems.begin(), testItems.end());
        if (!hasPath(grid)) {
            INFO("Part 2: %d,%d", bytes[i - 1].i, bytes[i - 1].j);
            return;
        }
    }
    ERROR("Wtf");
}

MAIN(part1, part2);