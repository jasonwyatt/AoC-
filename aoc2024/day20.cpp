#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <vector>

#include "tools/all.h"

using namespace tools::math;

std::vector<Vec2i> dirs = {
    Vec2i(1, 0),
    Vec2i(-1, 0),
    Vec2i(0, -1),
    Vec2i(0, 1)
};

struct Context {
    int width;
    int height;
    std::set<Vec2i> walls;
    std::vector<Vec2i> path;
    std::map<Vec2i, int> pathIndices;

    void fillInPath(Vec2i& start) {
        std::queue<Vec2i> queue;

        queue.push(start);
        while (!queue.empty()) {
            Vec2i current = queue.front();
            pathIndices[current] = path.size();
            path.push_back(current);
            queue.pop();

            for (Vec2i dir : dirs) {
                Vec2i candidate = current + dir;
                if (pathIndices.contains(candidate) || walls.contains(candidate)) continue;
                queue.push(candidate);
            }
        }
    }

    bool isValid(const Vec2i& point) {
        return point.i >= 0 && point.i < width && point.j >= 0 && point.j < height;
    }

    bool isValidStart(const Vec2i& point) {
        return isValid(point) && walls.contains(point);
    }

    bool isValidEnd(const Vec2i& point) {
        return isValid(point) && pathIndices.contains(point);
    }

    bool isValidCheat(const Vec2i& start, const Vec2i& end) {
        if (!isValid(start) || !isValidEnd(end)) return false;
        auto diff = end - start;
        int diffLen = abs(diff.i) + abs(diff.j);
        if (diffLen > 20) return false;
        return true;
    }
};

Context parse(std::ifstream& in) {
    Context result;
    int y = 0;
    Vec2i start;
    auto lambda = [&](std::string line) {
        result.width = line.size();
        for (int x = 0; x < line.size(); x++) {
            switch (line[x]) {
                case '#':
                    result.walls.insert(Vec2i(x, y));
                    break;
                case 'S':
                    start.i = x;
                    start.j = y;
                    break;
            }
        }
        y++;
        return true;
    };
    tools::readLines(in, lambda);
    result.fillInPath(start);
    result.height = y;
    return result;
}

void part1(std::ifstream& in) {
    Context context = parse(in);
    std::map<int, int> cheatSavingsCounts;

    int count = 0;
    for (int i = 0; i < context.path.size(); i++) {
        auto pathLoc = context.path[i];
        for (auto dir : dirs) {
            auto firstLoc = pathLoc + dir;
            auto secondLoc = firstLoc + dir;
            if (context.isValidStart(firstLoc) && context.isValidEnd(secondLoc)) {
                int endPathIndex = context.pathIndices[secondLoc];
                int savings = endPathIndex - i - 2;
                if (savings > 0) {
                    cheatSavingsCounts[savings]++;
                }
                if (savings >= 100) {
                    count++;
                }
            }
        }
    }
    INFO("Part 1: %d", count);
}

void part2(std::ifstream& in) {
    Context context = parse(in);
    std::map<int, int> cheatSavingsCounts;

    int count = 0;
    for (int i = 0; i < context.path.size(); i++) {
        auto firstLoc = context.path[i];
        for (int dy = -21; dy <= 21; dy++) {
            for (int dx = -21; dx <= 21; dx++) {
                auto secondLoc = firstLoc + Vec2i(dx, dy);
                if (context.isValidCheat(firstLoc, secondLoc)) {
                    auto dir = secondLoc - firstLoc;
                    int length = abs(dir.i) + abs(dir.j);
                    int endPathIndex = context.pathIndices[secondLoc];
                    int savings = endPathIndex - i - length;
                    if (endPathIndex > i && savings > 0) {
                        cheatSavingsCounts[savings]++;
                        if (savings >= 100) {
                            count++;
                        }
                    }
                }
            }
        }
    }
    INFO("Part 2: %d", count);
}

MAIN(part1, part2);