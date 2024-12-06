#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "absl/strings/str_split.h"
#include "tools/log.h"
#include "tools/main.h"

bool check(std::vector<std::pair<int, int>>& rules, std::vector<int>& path) {
    std::set<int> prefix;
    std::set<int> contains;
    for (int i : path) {
        contains.insert(i);
    }

    for (int i : path) {
        for (auto rule : rules) {
            if (rule.second == i) {
                if(!prefix.contains(rule.first) && contains.contains(rule.first)) {
                    return false;
                }
            }
        }
        prefix.insert(i);
    }
    return true;
}

void parseRules(std::ifstream& in, std::vector<std::pair<int, int>>& rulesOut) {
    std::string line;
    while (std::getline(in, line)) {
        if (line.length() == 0) {
            return;
        }

        std::vector<std::string> split = absl::StrSplit(line, "|");
        rulesOut.push_back(std::pair<int, int>(stoi(split[0]), stoi(split[1])));
    }
}

std::vector<int>* parseRoute(std::ifstream& in) {
    std::string line;
    if (std::getline(in, line)) {
        std::vector<std::string> split = absl::StrSplit(line, ",");
        std::vector<int>* pathOut = new std::vector<int>();
        for (std::string s : split) {
            pathOut->push_back(stoi(s));
        }
        return pathOut;
    }
    return nullptr;
}

void part1(std::ifstream& in) {
    std::vector<std::pair<int, int>> rules;
    parseRules(in, rules);

    int sum = 0;
    while (true) {
        auto path = parseRoute(in);
        if (path == nullptr) {
            break;
        }
        if (check(rules, *path)) {
            sum += (*path)[path->size() / 2];
        }
    }
    INFO("Part 1: %d", sum);
}

void findNext(std::set<std::pair<int, int>>& edges, std::set<int>& unvisited, std::set<int>& queue) {
    for (int node : unvisited) {
        bool isHead = true;
        for (auto edge : edges) {
            if (edge.second == node) {
                isHead = false;
                break;
            }
        }
        if (isHead) {
            queue.insert(node);
        }
    }
    for (int q : queue) {
        unvisited.erase(q);
    }
}

std::vector<int> fix(std::vector<std::pair<int, int>> rules, std::vector<int> path) {
    std::vector<int> result;
    std::set<int> contents;
    std::set<std::pair<int, int>> relatedPairs;
    for (int i : path) {
        contents.insert(i);
    }
    for (int i : path) {
        for (auto pair : rules) {
            if (!contents.contains(pair.first) || !contents.contains(pair.second)) {
                continue;
            }
            if (pair.first == i || pair.second == i) {
                relatedPairs.insert(pair);
            }
        }
    }

    std::set<int> queue;
    findNext(relatedPairs, contents, queue);

    while (!queue.empty()) {
        int head = *queue.begin();
        result.emplace(result.end(), head);
        queue.erase(head);
        std::set<std::pair<int, int>> edgesToDelete;
        for (auto pair : relatedPairs) {
            if (pair.first == head) {
                edgesToDelete.insert(pair);
            }
        }
        for (auto pair : edgesToDelete) {
            relatedPairs.erase(pair);
        }
        findNext(relatedPairs, contents, queue);
    }
    return result;
}

void part2(std::ifstream& in) {
    std::vector<std::pair<int, int>> rules;
    parseRules(in, rules);

    int sum = 0;
    while (true) {
        auto path = parseRoute(in);
        if (path == nullptr) {
            break;
        }
        if (!check(rules, *path)) {
            auto fixed = fix(rules, *path);
            sum += fixed[fixed.size() / 2];
        }
    }
    INFO("Part 2: %d", sum);
}

MAIN(part1, part2);
