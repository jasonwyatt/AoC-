#include <fstream>
#include <math.h>
#include <set>
#include <vector>

#include "tools/log.h"
#include "tools/main.h"

typedef std::vector<std::string> Board;

void print(Board& board) {
    for (auto line : board) {
        DEBUG("%s", line.c_str());
    }
}

int cast(Board& board, int w, int h, int i, int j, int di, int dj, bool found) {
    if (i + di < 0 || i + di >= h) return 0;
    if (j + dj < 0 || j + dj >= w) return 0;
    if (board[i + di][j + dj] == '#') {
        if (found) {
            board[i + di][j + dj] = 'x';
        } else {
            board[i + di][j + dj] = 'o';
        }
        return (found ? 0 : 1) + cast(board, w, h, i + di, j + dj, di, dj, true);
    }
    if (board[i + di][j + dj] != '.') {
        return 0;
    }
    return cast(board, w, h, i + di, j + dj, di, dj, found);
}

void reset(Board& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] != '.') {
                board[i][j] = '#';
            }
        }
    }
}

int scan(Board& board, int w, int h, int i, int j) {
    int total = 0;
    for (int di = 0; di < h; di++) {
        for (int dj = 0; dj < w; dj++) {
            if (di == 0 && dj == 0) continue;
            int found = cast(board, w, h, i, j, di, dj, false);
            total += found;
            if (found > 0) {
                DEBUG("Found %d with (%d, %d)", found, di, dj);
            }
        }
    }
    for (int di = 0; di > -h; di--) {
        for (int dj = 0; dj < w; dj++) {
            if (di == 0 && dj == 0) continue;
            int found = cast(board, w, h, i, j, di, dj, false);
            total += found;
            if (found > 0) {
                DEBUG("Found %d with (%d, %d)", found, di, dj);
            }
        }
    }
    for (int di = 0; di < h; di++) {
        for (int dj = 0; dj > -w; dj--) {
            if (di == 0 && dj == 0) continue;
            int found = cast(board, w, h, i, j, di, dj, false);
            total += found;
            if (found > 0) {
                DEBUG("Found %d with (%d, %d)", found, di, dj);
            }
        }
    }
    for (int di = 0; di > -h; di--) {
        for (int dj = 0; dj > -w; dj--) {
            if (di == 0 && dj == 0) continue;
            int found = cast(board, w, h, i, j, di, dj, false);
            total += found;
            if (found > 0) {
                DEBUG("Found %d with (%d, %d)", found, di, dj);
            }
        }
    }
    return total;
}

std::tuple<int, int, int> findMaxCountAndLoc(Board& board) {
    int w = board[0].size();
    int h = board.size();

    int max = 0;
    int maxI = 0;
    int maxJ = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (board[i][j] == '#') {
                int count = scan(board, w, h, i, j);
                if (count > max) {
                    maxI = i;
                    maxJ = j;
                    max = count;
                }
                DEBUG("After scanning from (%d, %d): %d", i, j, count);
                print(board);
                DEBUG("");
                reset(board);
            }
        }
    }
    return { max, maxI, maxJ };
}

void part1(std::ifstream& file) {
    LOG_LEVEL(ERROR_LEVEL);
    std::vector<std::string> board;

    while (true) {
        std::string* line = new std::string();
        if (!std::getline(file, *line)) {
            break;
        }
        board.push_back(*line);
    }

    auto max = std::get<0>(findMaxCountAndLoc(board));

    printf("Part1: %d\n", max);
}

std::tuple<double, double>* vaporize(
        std::set<std::tuple<double, double>>& asteroids,
        double iOrigin, double jOrigin,
        double di, double dj) {
    std::tuple<double, double>* asteroidToDelete = nullptr;
    double minDistanceHit = 10000000;
    for (auto asteroid : asteroids) {
        double asteroidVecI = std::get<0>(asteroid) - iOrigin;
        double asteroidVecJ = std::get<1>(asteroid) - jOrigin;
        if (abs(asteroidVecI) < 0.000001 && abs(asteroidVecJ) < 0.000001) continue;
        double dot = asteroidVecI * di + asteroidVecJ * dj;
        double dist = sqrt(asteroidVecI * asteroidVecI + asteroidVecJ * asteroidVecJ);
        if (abs(dot / dist - 1) < 0.001) {
            // hit!
            if (dist < minDistanceHit) {
                minDistanceHit = dist;
                asteroidToDelete = new std::tuple<double, double>(asteroid);
            }
        }
    }

    return asteroidToDelete;
}

void part2(std::ifstream& file) {
    Board board;
    std::set<std::tuple<double, double>> asteroids;

    while (true) {
        std::string* line = new std::string();
        if (!std::getline(file, *line)) {
            break;
        }
        board.push_back(*line);
    }

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (board[i][j] == '#') {
                asteroids.insert({i, j});
            }
        }
    }

    auto max = findMaxCountAndLoc(board);

    LOG_LEVEL(DEBUG_LEVEL);

    double iOrigin = std::get<1>(max);
    double jOrigin = std::get<2>(max);
    DEBUG("Set has size %ld, start at (%f, %f)", asteroids.size(), iOrigin, jOrigin);

    int count = 0;
    double theta = 0;

    std::set<std::tuple<double, double>> toDelete;
    while (count < 200 && asteroids.size() > 1) {
        double di = -cos(theta);
        double dj = sin(theta);

        auto deleted = vaporize(asteroids, iOrigin, jOrigin, di, dj);
        if (deleted != nullptr) {
            if (!toDelete.contains(*deleted)) {
                count++;
                toDelete.insert(*deleted);
                DEBUG("will delete #%d at (%f, %f) with (%f, %f, %f), remaining: %ld, to delete: %ld",
                    count,
                    std::get<0>(*deleted), std::get<1>(*deleted),
                    di, dj,
                    theta,
                    asteroids.size(),
                    toDelete.size());
            }
        }
        int loopsBefore = theta / (M_PI * 2);
        theta += 0.00001;
        int loopsAfter = (int)(theta / (M_PI * 2));
        if (loopsAfter > loopsBefore) {
            DEBUG("Deleting");
            // delete the toDelete.
            for (auto deletable : toDelete) {
                asteroids.erase(deletable);
            }
            toDelete.clear();
        }
    }
}

MAIN(part1, part2);
