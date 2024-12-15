#include <fstream>
#include <set>
#include <thread>
#include <vector>
#include <sstream>
#include <iostream>

#include "tools/log.h"
#include "tools/main.h"
#include "tools/math/vec2.h"

using namespace tools::math;
using namespace std;

Vec2<int> up(-1, 0);
Vec2<int> down(1, 0);
Vec2<int> right(0, 1);
Vec2<int> left(0, -1);
Vec2<int>& toVec(char dir) {
    switch (dir) {
        case '^': return ::up;
        case 'v': return ::down;
        case '<': return ::left;
        case '>': return ::right;
    }
    ERROR("wtf, bad dir: %c", dir);
}

struct Context {
    int w;
    int h;
    Vec2<int> robot;
    set<Vec2<int>> walls;
    set<Vec2<int>> packages;

    bool movePackages(Vec2<int>& package, Vec2<int>& dir) {
        Vec2<int> next = package + dir;
        if (walls.contains(next)) return false;
        if (packages.contains(next)) {
            if (movePackages(next, dir)) {
                packages.erase(package);
                packages.insert(next);
                return true;
            }
            return false;
        }
        packages.erase(package);
        packages.insert(next);
        return true;
    }

    void move(Vec2<int>& dir) {
        Vec2<int> current = robot + dir;
        if (walls.contains(current)) return;
        if (packages.contains(current) && movePackages(current, dir)) {
            robot += dir;
        } else if (!packages.contains(current)) {
            robot += dir;
        }
    }

    void print() {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                Vec2<int> pos(i, j);
                if (robot == pos) {
                    cout << "@";
                } else if (walls.contains(pos)) {
                    cout << "#";
                } else if (packages.contains(pos)) {
                    cout << "O";
                } else {
                    cout << " ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
};

struct Moves {
    set<Vec2<int>> leftOriginals;
    set<Vec2<int>> leftNexts;
    set<Vec2<int>> rightOriginals;
    set<Vec2<int>> rightNexts;
};

struct Context2 {
    int w;
    int h;
    Vec2<int> robot;
    set<Vec2<int>> walls;
    set<Vec2<int>> packagesLeft;
    set<Vec2<int>> packagesRight;

    bool movePackageLeft(Vec2<int>& leftPackage, Vec2<int>& dir, Moves& out, bool skipPartner) {
        DEBUG("movePackageLeft(%s, %s, .., %i)", leftPackage.toString().c_str(), dir.toString().c_str(), skipPartner);
        Vec2<int> next = leftPackage + dir;
        Vec2<int> rightPackage = leftPackage + ::right;
        if (walls.contains(next)) return false;
        if (!skipPartner && !movePackageRight(rightPackage, dir, out, true)) return false;
        if (packagesLeft.contains(next) && !movePackageLeft(next, dir, out, false)) return false;
        if (dir != ::right && packagesRight.contains(next) && !movePackageRight(next, dir, out, false)) return false;

        out.leftOriginals.insert(leftPackage);
        out.leftNexts.insert(next);
        return true;
    }

    bool movePackageRight(Vec2<int>& rightPackage, Vec2<int>& dir, Moves& out, bool skipPartner) {
        DEBUG("movePackageRight(%s, %s, .., %i)", rightPackage.toString().c_str(), dir.toString().c_str(), skipPartner);
        Vec2<int> next = rightPackage + dir;
        Vec2<int> leftPackage = rightPackage + ::left;
        if (walls.contains(next)) return false;
        if (!skipPartner && !movePackageLeft(leftPackage, dir, out, true)) return false;
        if (dir != ::left && packagesLeft.contains(next) && !movePackageLeft(next, dir, out, false)) return false;
        if (packagesRight.contains(next) && !movePackageRight(next, dir, out, false)) return false;

        out.rightOriginals.insert(rightPackage);
        out.rightNexts.insert(next);
        return true;
    }

    void commit(Moves& moves) {
        for (auto& rightOriginal : moves.rightOriginals) {
            packagesRight.erase(rightOriginal);
        }
        for (auto& leftOriginal : moves.leftOriginals) {
            packagesLeft.erase(leftOriginal);
        }
        for (auto& rightNext : moves.rightNexts) {
            packagesRight.insert(rightNext);
        }
        for (auto& leftNext : moves.leftNexts) {
            packagesLeft.insert(leftNext);
        }
    }

    void move(Vec2<int>& dir) {
        DEBUG("move(%s), robot=%s", dir.toString().c_str(), robot.toString().c_str());
        Vec2<int> current = robot + dir;
        if (walls.contains(current)) return;
        Moves moves;
        if (packagesLeft.contains(current) && movePackageLeft(current, dir, moves, false)) {
            commit(moves);
            robot += dir;
        } else if (packagesRight.contains(current) && movePackageRight(current, dir, moves, false)) {
            commit(moves);
            robot += dir;
        } else if (!packagesLeft.contains(current) && !packagesRight.contains(current)) {
            robot += dir;
        }
    }

    void print() {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                Vec2<int> pos(i, j);
                if (robot == pos) {
                    cout << "@";
                } else if (walls.contains(pos)) {
                    cout << "#";
                } else if (packagesLeft.contains(pos)) {
                    cout << "[";
                } else if (packagesRight.contains(pos)) {
                    cout << "]";
                } else {
                    cout << " ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
};

Context parse(std::ifstream& in) {
    string line;
    Context c;
    int i = 0;
    while (getline(in, line)) {
        if (line.size() == 0) break;
        c.w = line.size();
        for (int j = 0; j < line.size(); j++) {
            if (line[j] == '#') {
                c.walls.insert(Vec2<int>(i, j));
            } else if (line[j] == '@') {
                c.robot.i = i;
                c.robot.j = j;
            } else if (line[j] == 'O') {
                c.packages.insert(Vec2<int>(i, j));
            }
        }
        i++;
    }
    c.h = i;
    return c;
}

Context2 parse2(std::ifstream& in) {
    string line;
    Context2 c;
    int i = 0;
    while (getline(in, line)) {
        if (line.size() == 0) break;
        c.w = line.size() * 2;
        for (int j = 0; j < line.size() * 2; j += 2) {
            if (line[j / 2] == '#') {
                c.walls.insert(Vec2<int>(i, j));
                c.walls.insert(Vec2<int>(i, j + 1));
            } else if (line[j / 2] == '@') {
                c.robot.i = i;
                c.robot.j = j;
            } else if (line[j / 2] == 'O') {
                c.packagesLeft.insert(Vec2<int>(i, j));
                c.packagesRight.insert(Vec2<int>(i, j + 1));
            }
        }
        i++;
    }
    c.h = i;
    return c;
}

void part1(std::ifstream& in) {
    Context context = parse(in);

    string line;
    while (getline(in, line)) {
        for (char c : line) {
            context.move(toVec(c));
        }
    }

    long total = 0;
    for (auto& package : context.packages) {
        total += (package.i * 100 + package.j);
    }

    INFO("Part 1: %ld", total);
}

void part2(std::ifstream& in) {
    LOG_LEVEL(INFO_LEVEL);
    Context2 context = parse2(in);

    string line;
    bool animate = false;
    int i = 0;
    while (getline(in, line)) {
        for (char c : line) {
            //sofar << c;
            context.move(toVec(c));
            if (animate && i % 50 == 0) {
                cout << "After " << c << endl;
                context.print();
                this_thread::sleep_for(chrono::milliseconds(16));
            }
            i++;
        }
    }

    long total = 0;
    for (auto& package : context.packagesLeft) {
        total += (package.i * 100 + package.j);
    }

    INFO("Part 2: %ld", total);
}

MAIN(part1, part2);