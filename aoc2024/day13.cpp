#include <fstream>
#include <regex>

#include "tools/log.h"
#include "tools/main.h"

struct Button {
    long dx;
    long dy;
};

struct Solution {
    long a;
    long b;
};

struct Problem {
    Button a;
    Button b;
    long targetX;
    long targetY;
};

std::regex buttonPattern("Button .: X\\+(\\d+), Y\\+(\\d+)");
std::regex prizePattern("Prize: X=(\\d+), Y=(\\d+)");

bool parseButton(std::ifstream& in, Button& out) {
    std::string line;
    if (!std::getline(in, line)) return false;
    auto x = std::sregex_iterator(line.begin(), line.end(), buttonPattern);
    std::smatch results = *x;
    out.dx = stoi(results[1]);
    out.dy = stoi(results[2]);
    return true;
}

bool parseProblem(std::ifstream& in, Problem& out) {
    if (!parseButton(in, out.a)) return false;
    if (!parseButton(in, out.b)) return false;

    std::string line;
    if (!std::getline(in, line)) return false;

    std::smatch results;
    if (!std::regex_match(line, results, prizePattern)) return false;
    out.targetX = stoi(results[1]);
    out.targetY = stoi(results[2]);

    return true;
}

bool solve(Problem& problem, Solution& solution) {
/*
Button A: X+94, Y+34
Button B: X+22, Y+67
Prize: X=8400, Y=5400

Becomes

P_a * 94 + P_b * 22 = 8400
P_a * 34 + P_b * 67 = 5400

Generalized:

P_x * A + P_y * B = C
P_x * D + P_y * E = F

P_x + P_y * (B/A) = C/A
P_x + P_y * (E/D) = F/D
P_y * (B/A - E/D) = (F/A - F/D)
P_y * (BD - EA) / D = (CD - FA) / D
P_y = (CD - FA) / (BD - EA)

P_x * A + P_y * B = C
P_x = (C - P_y * B) / A
*/

    long A = problem.a.dx;
    long B = problem.b.dx;
    long C = problem.targetX;

    long D = problem.a.dy;
    long E = problem.b.dy;
    long F = problem.targetY;

    double bPresses = (double)(C * D - F * A) / ((double)B * D - E * A);
    double aPresses = ((double) C - bPresses * B) / A;
    if (abs(bPresses - (long)bPresses) > 0.0001 || bPresses < 0) {
        return false;
    }
    if (abs(aPresses - (long)aPresses) > 0.0001 || aPresses < 0) {
        return false;
    }
    solution.a = (long) aPresses;
    solution.b = (long) bPresses;
    return true;
}

void part1(std::ifstream& in) {
    std::string line;
    Problem problem;
    long tokens = 0;
    while (parseProblem(in, problem)) {
        Solution solution;
        if (solve(problem, solution)) {
            tokens += solution.a * 3;
            tokens += solution.b;
        }

        if (!std::getline(in, line)) break;
    }
    INFO("Part 1: %ld", tokens);
}

void part2(std::ifstream& in) {
    std::string line;
    Problem problem;
    long tokens = 0;
    while (parseProblem(in, problem)) {
        problem.targetX += 10000000000000;
        problem.targetY += 10000000000000;
        Solution solution;
        if (solve(problem, solution)) {
            tokens += solution.a * 3;
            tokens += solution.b;
        }

        if (!std::getline(in, line)) break;
    }
    INFO("Part 2: %ld", tokens);
}

MAIN(part1, part2);
