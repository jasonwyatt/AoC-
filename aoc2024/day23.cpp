#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>

#include "tools/all.h"

typedef std::set<std::string> Trio;
typedef std::set<std::string> Edge;

void getTrios(std::map<std::string, std::set<Edge>>& allEdges,
              const std::string& start,
              const std::string& pos,
              int jumps,
              std::set<Edge>& edgesInUse,
              std::set<std::set<std::string>>& result) {
    if (start == pos && jumps == 3) {
        std::set<std::string> resultNodes;
        for (auto& edge : edgesInUse) {
            for (auto& node : edge) {
                resultNodes.insert(node);
            }
        }
        if (!result.contains(resultNodes)) {
            std::ostringstream trioStr;
            for (auto& node : resultNodes) {
                trioStr << node << " ";
            }
            result.insert(resultNodes);
        }
        return;
    }
    if (jumps == 3) return;

    auto& edgesWithPos = allEdges[pos];
    for (auto& edge : edgesWithPos) {
        if (edgesInUse.contains(edge)) continue;
        edgesInUse.insert(edge);
        for (auto& node : edge) {
            if (node == pos) continue;
            getTrios(allEdges, start, node, jumps + 1, edgesInUse, result);
        }
        edgesInUse.erase(edge);
    }
}

void part1(std::ifstream& in) {
    std::set<std::string> nodes;
    std::map<std::string, std::set<Edge>> edgesByNode;

    auto lambda = [&](std::string line) {
        auto neighbors = tools::split(line, "-");
        Edge edge = {neighbors[0], neighbors[1]};
        nodes.insert(neighbors[0]);
        nodes.insert(neighbors[1]);
        edgesByNode[neighbors[0]].insert(edge);
        edgesByNode[neighbors[1]].insert(edge);
        return true;
    };
    tools::readLines(in, lambda);

    std::set<std::set<std::string>> trios;

    std::set<Edge> dummy;
    for (auto& node : nodes) {
        getTrios(edgesByNode, node, node, 0, dummy, trios);
    }

    int count = 0;
    for (auto& trio : trios) {
        bool found = false;
        for (auto& name : trio) {
            if (name[0] == 't') {
                found = true;
            }
        }
        if (found) count++;
    }
    INFO("Part 1: %d", count);
}

std::string stringify(std::set<std::string>& in) {
    std::ostringstream out;
    bool first = true;
    for (auto& s : in) {
        if (!first) {
            out << ",";
        }
        out << s;
    }
    return out.str().c_str();
}

typedef std::map<std::string, std::set<std::string>> NeighborsByNode;

std::map<std::set<std::string>, std::set<std::string>> cache;
std::set<std::string> tightFrom(const std::string& origin, NeighborsByNode& neighborsByNode, std::set<std::string>& soFar) {
    if (cache.contains(soFar)) return cache[soFar];
    int maxSize = soFar.size();
    std::set<std::string> max = soFar;
    for (auto& neighbor : neighborsByNode[origin]) {
        if (soFar.contains(neighbor)) continue;
        bool isGood = true;
        for (auto& soFarNode : soFar) {
            if (!neighborsByNode[neighbor].contains(soFarNode)) {
                isGood = false;
                break;
            }
        }
        if (isGood) {
            soFar.insert(neighbor);
            auto withIt = tightFrom(neighbor, neighborsByNode, soFar);
            if (maxSize < withIt.size()) {
                max = withIt;
                maxSize = withIt.size();
            }
            soFar.erase(neighbor);
        }
    }
    cache[soFar] = max;
    return max;
}

void part2(std::ifstream& in) {
    std::map<std::string, std::set<Edge>> edgesByNode;
    NeighborsByNode neighborsByNode;

    auto lambda = [&](std::string line) {
        auto neighbors = tools::split(line, "-");
        Edge edge = {neighbors[0], neighbors[1]};
        edgesByNode[neighbors[0]].insert(edge);
        edgesByNode[neighbors[1]].insert(edge);
        neighborsByNode[neighbors[0]].insert(neighbors[0]);
        neighborsByNode[neighbors[0]].insert(neighbors[1]);
        neighborsByNode[neighbors[1]].insert(neighbors[0]);
        neighborsByNode[neighbors[1]].insert(neighbors[1]);
        return true;
    };
    tools::readLines(in, lambda);

    int maxSize = 0;
    std::set<std::string> maxParty;
    for (auto& [node, neighbors] : neighborsByNode) {
        std::set<std::string> begin = {node};
        auto maxPartyForNode = tightFrom(node, neighborsByNode, begin);
        if (maxPartyForNode.size() > maxSize) {
            maxSize = maxPartyForNode.size();
            maxParty = maxPartyForNode;
        }
    }
    std::ostringstream result;
    bool first = true;
    for (auto& node : maxParty) {
        if (!first) {
            result << ",";
        }
        result << node;
        first = false;
    }
    INFO("Part 2: %s", result.str().c_str());
}

MAIN(part1, part2);