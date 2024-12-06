#include <fstream>
#include <set>
#include <tuple>

#include "tools/log.h"
#include "tools/main.h"

void getDxDy(int dir, int& dx, int& dy) {
    switch (dir) {
        case 0:
            dx = 0;
            dy = -1;
            break;
        case 1:
            dx = 1;
            dy = 0;
            break;
        case 2:
            dx = 0;
            dy = 1;
            break;
        case 3:
            dx = -1;
            dy = 0;
            break;
    }
}

class Board {
public:
    Board() = default;
    ~Board() = default;

    int width;
    int height;
    std::set<std::pair<int, int>> barriers;
    std::set<std::pair<int, int>> guardVisited;
    std::set<std::tuple<int, int, int>> guardVisited2;
    std::pair<int, int> guardLoc;
    int guardDir;

    void parse(std::ifstream& in) {
        std::string line;
        int i = 0;
        while (std::getline(in, line)) {
            width = line.size();
            for (int j = 0; j < line.size(); j++) {
                if (line[j] == '#') {
                    barriers.insert(std::pair<int, int>(i, j));
                } else if (line[j] == '^') {
                    guardLoc = std::pair<int, int>(i, j);
                    guardDir = 0;
                } else if (line[j] == '>') {
                    guardLoc = std::pair<int, int>(i, j);
                    guardDir = 1;
                } else if (line[j] == 'v') {
                    guardLoc = std::pair<int, int>(i, j);
                    guardDir = 2;
                } else if (line[j] == '<') {
                    guardLoc = std::pair<int, int>(i, j);
                    guardDir = 3;
                }
            }
            i++;
        }
        height = i;
        guardVisited.insert(guardLoc);
    };

    bool tick() {
        if (guardLoc.first < 0 || guardLoc.first >= height) return false;
        if (guardLoc.second < 0 || guardLoc.second >= width) return false;
        guardVisited.insert(guardLoc);

        int dx = 0;
        int dy = 0;
        getDxDy(guardDir, dx, dy);
        std::pair<int, int> next = { guardLoc.first + dy, guardLoc.second + dx };
        if (barriers.contains(next)) {
            guardDir = (guardDir + 1) % 4;
            return true;
        }
        guardLoc = next;
        return true;
    };

    bool tick2(bool& foundLoop) {
        if (guardLoc.first < 0 || guardLoc.first >= height) return false;
        if (guardLoc.second < 0 || guardLoc.second >= width) return false;

        std::tuple<int, int, int> locDir = {guardLoc.first, guardLoc.second, guardDir};
        if (guardVisited2.contains(locDir)) {
            foundLoop = true;
            return false;
        }
        foundLoop = false;
        guardVisited2.insert(locDir);

        int dx = 0;
        int dy = 0;
        getDxDy(guardDir, dx, dy);
        std::pair<int, int> next = { guardLoc.first + dy, guardLoc.second + dx };
        if (barriers.contains(next)) {
            guardDir = (guardDir + 1) % 4;
            return true;
        }
        guardLoc = next;
        return true;
    };
};

void part1(std::ifstream& in) {
    Board board;
    board.parse(in);

    while (board.tick()) {
        // do the tick.
    }
    INFO("Part 1: %ld", board.guardVisited.size());
}

void part2(std::ifstream& in) {
    Board board;
    board.parse(in);

    std::pair<int, int> originalGuardLoc = { board.guardLoc.first, board.guardLoc.second };
    int originalGuardDir = board.guardDir;
    int total = 0;
    for (int i = 0; i < board.height; i++) {
        for (int j = 0; j < board.height; j++) {
            std::pair<int, int> pos = { i, j };
            if (!board.barriers.contains(pos) && originalGuardLoc != pos) {
                board.barriers.insert(pos);
                board.guardLoc = originalGuardLoc;
                board.guardDir = originalGuardDir;
                board.guardVisited2.clear();

                bool foundLoop = false;
                while (board.tick2(foundLoop)) {
                    // do the tick.
                }
                if (foundLoop) {
                    total++;
                }

                board.barriers.erase(pos);
            }
        }
    }
    INFO("Part 2: %d", total);
}

MAIN(part1, part2);