#include <fstream>
#include <thread>
#include <vector>
#include <iostream>
#include <regex>
#include <vector>

#include "tools/log.h"
#include "tools/main.h"
#include "tools/math/vec2.h"

using namespace tools::math;

typedef struct {
    Vec2<int> velocity;
    Vec2<int> initialPosition;
} Robot;

Vec2<int> getFinalPosition(Robot& robot, Vec2<int>& boardSize, int seconds) {
    Vec2<int> unmoddedLocation = robot.initialPosition + (robot.velocity * seconds);
    return Vec2<int>(
        (unmoddedLocation.i % boardSize.i + boardSize.i) % boardSize.i,
        (unmoddedLocation.j % boardSize.j + boardSize.j) % boardSize.j);
}

std::regex pattern("p=(\\d+),(\\d+) v=(-?\\d+),(-?\\d+)");

Robot parse(std::string& line) {
    std::smatch match;
    std::regex_match(line, match, pattern);
    return Robot {
        .velocity = Vec2<int>(stoi(match[4]), stoi(match[3])),
        .initialPosition = Vec2<int>(stoi(match[2]), stoi(match[1]))
    };
}

bool worthPrinting(int* robots, int width, int height) {
    int maxRun = 0;
    for (int i = 0; i < height; i++) {
        int run = 0;
        for (int j = 0; j < width; j++) {
            int pos = i * width + j;
            if (robots[pos] > 0) {
                run++;
            } else {
                maxRun = std::max(maxRun, run);
                run = 0;
            }
        }
    }
    return maxRun > 10;
}

void printRobots(int* robots, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pos = i * width + j;
            if (robots[pos] > 0) {
                std::cout << "X";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

int safetyFactor(int* robots, int width, int height) {
    int topLeft = 0;
    int topRight = 0;
    int bottomLeft = 0;
    int bottomRight = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i < height / 2) {
                if (j < width / 2) {
                    topLeft += robots[i * width + j];
                } else if (j > width / 2) {
                    topRight += robots[i * width + j];
                }
            } else if (i > height / 2) {
                if (j < width / 2) {
                    bottomLeft += robots[i * width + j];
                } else if (j > width / 2) {
                    bottomRight += robots[i * width + j];
                }
            }
        }
    }
    return topLeft * topRight * bottomLeft * bottomRight;
}

void part1(std::ifstream& in) {
    Vec2<int> boardSize(103, 101);
    int simulationSeconds = 100;
    int* robots = (int*) calloc(boardSize.i * boardSize.j, sizeof(int));

    std::string line;
    for (int i = 0; i < boardSize.i; i++) {
        for (int j = 0; j < boardSize.j; j++) {
            robots[i * boardSize.j + j] = 0;
        }
    }
    while (std::getline(in, line)) {
        Robot r = parse(line);
        auto position = getFinalPosition(r, boardSize, simulationSeconds);
        robots[position.i * boardSize.j + position.j]++;
    }

    INFO("Part 1: %d", safetyFactor(robots, boardSize.j, boardSize.i));
}

/**
 * Note: I hate this one. There isn't a good definition of what the tree is that we're looking for.
 * You just have to run it a bunch of times and hope you see what you're expecting.
 */
void part2(std::ifstream& in) {
    Vec2<int> boardSize(103, 101);
    int* robotCounts = (int*) calloc(boardSize.i * boardSize.j, sizeof(int));
    std::vector<Robot> robots;
    std::string line;
    while (std::getline(in, line)) {
        robots.push_back(parse(line));
    }

    bool keepGoing = true;
    int simulationSeconds = 0;
    while (keepGoing && simulationSeconds < 10001) {
        for (int i = 0; i < boardSize.i; i++) {
            for (int j = 0; j < boardSize.j; j++) {
                robotCounts[i * boardSize.j + j] = 0;
            }
        }
        for (Robot r : robots) {
            auto position = getFinalPosition(r, boardSize, simulationSeconds);
            robotCounts[position.i * boardSize.j + position.j]++;
        }
        if (worthPrinting(robotCounts, boardSize.j, boardSize.i)) {
            printf("\n");
            printf("Seconds: %d\n", simulationSeconds);
            printRobots(robotCounts, boardSize.j, boardSize.i);
        }
        simulationSeconds++;
    }

    INFO("Part 2: %d", safetyFactor(robotCounts, boardSize.j, boardSize.i));
}

MAIN(part1, part2);