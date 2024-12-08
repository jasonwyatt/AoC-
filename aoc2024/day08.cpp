#include <fstream>
#include <vector>
#include <map>
#include <tuple>

#include "tools/log.h"
#include "tools/main.h"

typedef std::tuple<int, int> vec;
typedef std::tuple<int, int, char> antenna;

typedef struct {
    std::vector<antenna> antennae;
    int width;
    int height;
} Board;

template <class T, class U>
bool isColinear(T& a, U& b) {
    return (std::get<0>(a) * std::get<1>(b) - std::get<1>(a) * std::get<0>(b)) == 0;
}

template <class T, class U>
vec sub(T& a, U& b) {
    return { std::get<0>(b) - std::get<0>(a), std::get<1>(b) - std::get<1>(a) };
}

template <class T>
int l1Norm(T& v) {
    return std::get<0>(v) + std::get<1>(v);
}

bool isAntinode(vec& loc, antenna& a, antenna& b) {
    if (std::get<2>(a) != std::get<2>(b)) return false;

    vec av = sub(a, loc);
    vec bv = sub(b, loc);

    if (!isColinear(av, bv)) return false;
    int avNorm = l1Norm(av);
    int bvNorm = l1Norm(bv);
    return avNorm == 2 * bvNorm || 2 * avNorm == bvNorm;
}

bool isAntinode(vec& loc, std::vector<antenna>& antennae) {
    for (antenna a : antennae) {
        for (antenna b : antennae) {
            if (a == b) continue;
            if (isAntinode(loc, a, b)) {
                return true;
            }
        }
    }
    return false;
}

bool isAntinode2(vec& loc, antenna& a, antenna& b) {
    if (std::get<2>(a) != std::get<2>(b)) return false;

    vec av = sub(a, loc);
    vec bv = sub(b, loc);

    if (!isColinear(av, bv)) return false;
    return true;
}

bool isAntinode2(vec& loc, std::vector<antenna>& antennae) {
    for (antenna a : antennae) {
        for (antenna b : antennae) {
            if (a == b) continue;
            if (isAntinode2(loc, a, b)) {
                return true;
            }
        }
    }
    return false;
}

Board parse(std::ifstream& in) {
    std::vector<antenna> antennae;
    std::string line;
    int i = 0;
    int width = 0;
    while (std::getline(in, line)) {
        for (int j = 0; j < line.size(); j++) {
            if (line[j] != '.') {
                antennae.push_back({i, j, line[j]});
            }
        }
        width = line.size();
        i++;
    }
    return Board { antennae, width, i };
}

void part1(std::ifstream& in) {
    Board board = parse(in);

    int count = 0;
    for (int i = 0; i < board.height; i++) {
        for (int j = 0; j < board.width; j++) {
            vec pos = { i, j };
            if (isAntinode(pos, board.antennae)) {
                count++;
            }
        }
    }
    INFO("Part 1: %d", count);
}

void part2(std::ifstream& in) {
    Board board = parse(in);

    int count = 0;
    for (int i = 0; i < board.height; i++) {
        for (int j = 0; j < board.width; j++) {
            vec pos = { i, j };
            if (isAntinode2(pos, board.antennae)) {
                count++;
            }
        }
    }
    INFO("Part 2: %d", count);
}

MAIN(part1, part2);