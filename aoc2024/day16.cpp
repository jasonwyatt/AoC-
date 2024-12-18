#include <fstream>
#include <set>
#include <queue>
#include <tuple>
#include <map>

#include "tools/io.h"
#include "tools/log.h"
#include "tools/main.h"
#include "tools/math/vec2.h"

using namespace tools::math;

typedef std::set<Vec2<int>> Walls;

typedef std::tuple<int, int, int> VisitedKey;
typedef std::map<VisitedKey, long> Visited;

Vec2<int> orientations[] = {
    Vec2<int>(-1, 0),
    Vec2<int>(0, 1),
    Vec2<int>(1, 0),
    Vec2<int>(0, -1),
};

struct Reindeer {
    Vec2<int> position;
    long score;
    int orientationIndex;

    bool canRotateClockwise(Visited& visited, long bestScore) {
        VisitedKey key{position.i, position.j, ((orientationIndex + 1) % 4 + 4) % 4};
        if (visited.contains(key)) {
            return bestScore >= score + 1000 && visited[key] >= score + 1000;
        } else {
            return bestScore >= score + 1000;
        }
    }

    Reindeer rotateClockwise() {
        return Reindeer {
            .position = position,
            .score = score + 1000,
            .orientationIndex = ((orientationIndex + 1) % 4 + 4) % 4,
        };
    }

    bool canRotateCounterClockwise(Visited& visited, long bestScore) {
        VisitedKey key{position.i, position.j, ((orientationIndex - 1) % 4 + 4) % 4};
        if (visited.contains(key)) {
            return bestScore >= score + 1000 && visited[key] >= score + 1000;
        } else {
            return bestScore >= score + 1000;
        }
    }

    Reindeer rotateCounterClockwise() {
        return Reindeer {
            .position = position,
            .score = score + 1000,
            .orientationIndex = ((orientationIndex - 1) % 4 + 4) % 4,
        };
    }

    bool canMove(Walls& walls, Visited& visited, long bestScore) {
        auto next = position + orientations[orientationIndex];
        VisitedKey key{next.i, next.j, orientationIndex};
        if (visited.contains(key)) {
            return bestScore >= score + 1 && visited[key] >= score + 1;
        } else {
            return bestScore >= score + 1 && !walls.contains(next);
        }
    }

    Reindeer move() {
        return Reindeer {
            .position = position + orientations[orientationIndex],
            .score = score + 1,
            .orientationIndex = orientationIndex,
        };
    }
};

void part1(std::ifstream& in) {
    Walls walls;
    Vec2<int> start;
    Vec2<int> end;

    int i = 0;
    auto reader = [&](std::string line) {
        for (int j = 0; j < line.size(); j++) {
            Vec2<int> position(i, j);
            if (line[j] == '#') {
                walls.insert(position);
            } else if (line[j] == 'S') {
                start.i = position.i;
                start.j = position.j;
            } else if (line[j] == 'E') {
                end.i = position.i;
                end.j = position.j;
            }
        }
        i++;
        return true;
    };
    tools::readLines(in, reader);

    long minScore = LONG_MAX;
    Visited visited;
    auto cmp = [end](Reindeer& left, Reindeer& right) {
        return (left.position - end).mag() < (right.position - end).mag();
    };
    std::priority_queue<Reindeer, std::vector<Reindeer>, decltype(cmp)> queue(cmp);
    Reindeer startReindeer{
        .position = start,
        .score = 0,
        .orientationIndex = 1,
    };
    queue.push(startReindeer);
    visited[{startReindeer.position.i, startReindeer.position.j, startReindeer.orientationIndex}] = 0;
    while (!queue.empty()) {
        Reindeer current = queue.top();
        queue.pop();
        if (current.score > minScore) continue;
        if (current.position == end) {
            minScore = std::min(current.score, minScore);
            continue;
        }
        if (current.canMove(walls, visited, minScore)) {
            Reindeer next = current.move();
            queue.push(next);
            visited[{next.position.i, next.position.j, next.orientationIndex}] = next.score;
        }
        if (current.canRotateClockwise(visited, minScore)) {
            Reindeer next = current.rotateClockwise();
            queue.push(next);
            visited[{next.position.i, next.position.j, next.orientationIndex}] = next.score;
        }
        if (current.canRotateCounterClockwise(visited, minScore)) {
            Reindeer next = current.rotateCounterClockwise();
            queue.push(next);
            visited[{next.position.i, next.position.j, next.orientationIndex}] = next.score;
        }
    }
    INFO("Part 1: %ld", minScore);
}

void part2(std::ifstream& in) {
    Walls walls;
    Vec2<int> start;
    Vec2<int> end;

    int i = 0;
    auto reader = [&](std::string line) {
        for (int j = 0; j < line.size(); j++) {
            Vec2<int> position(i, j);
            if (line[j] == '#') {
                walls.insert(position);
            } else if (line[j] == 'S') {
                start.i = position.i;
                start.j = position.j;
            } else if (line[j] == 'E') {
                end.i = position.i;
                end.j = position.j;
            }
        }
        i++;
        return true;
    };
    tools::readLines(in, reader);

    long minScore = LONG_MAX;
    Visited visited;
    auto cmp = [end](std::vector<Reindeer>& left, std::vector<Reindeer>& right) {
        return (left.back().position - end).mag() < (right.back().position - end).mag();
    };
    std::priority_queue<std::vector<Reindeer>, std::vector<std::vector<Reindeer>>, decltype(cmp)> queue(cmp);
    Reindeer startReindeer{
        .position = start,
        .score = 0,
        .orientationIndex = 1,
    };
    queue.push({startReindeer});
    visited[{startReindeer.position.i, startReindeer.position.j, startReindeer.orientationIndex}] = 0;

    std::map<long, std::set<Vec2<int>>> goodSeats;

    while (!queue.empty()) {
        std::vector<Reindeer> current = queue.top();
        queue.pop();
        if (current.back().score > minScore) continue;
        if (current.back().position == end) {
            long score = current.back().score;
            if (!goodSeats.contains(score)) {
                goodSeats[score] = {};
            }
            for (auto pos : current) {
                goodSeats[score].insert(pos.position);
            }
            minScore = std::min(current.back().score, minScore);
            continue;
        }
        if (current.back().canMove(walls, visited, minScore)) {
            Reindeer next = current.back().move();
            std::vector<Reindeer> nextList = current;
            nextList.push_back(next);
            queue.push(nextList);
            visited[{next.position.i, next.position.j, next.orientationIndex}] = next.score;
        }
        if (current.back().canRotateClockwise(visited, minScore)) {
            Reindeer next = current.back().rotateClockwise();
            std::vector<Reindeer> nextList = current;
            nextList.push_back(next);
            queue.push(nextList);
            visited[{next.position.i, next.position.j, next.orientationIndex}] = next.score;
        }
        if (current.back().canRotateCounterClockwise(visited, minScore)) {
            Reindeer next = current.back().rotateCounterClockwise();
            std::vector<Reindeer> nextList = current;
            nextList.push_back(next);
            queue.push(nextList);
            visited[{next.position.i, next.position.j, next.orientationIndex}] = next.score;
        }
    }
    INFO("Part 2: %ld", goodSeats[minScore].size());
}

MAIN(part1, part2);