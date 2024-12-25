#include <algorithm>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <regex>

#include "tools/all.h"

struct Node;
bool readNode(Node* node);
void resetNode(Node* node);
void validateNode(Node* node);

enum GateType {
    AND,
    OR,
    XOR,
    INIT,
};
GateType gateTypeFromString(const std::string& str) {
    if (str == "AND") return AND;
    if (str == "OR") return OR;
    if (str == "XOR") return XOR;
    throw std::invalid_argument("Invalid gate type");
}

struct Gate {
    int id;
    GateType type;
    bool hasValue;
    bool value;
    Node* out = nullptr;
    Node* lhs = nullptr;
    Node* rhs = nullptr;

    bool read() {
        if (type == INIT) return value;
        bool readValue;
        switch (type) {
            case AND:
                readValue = readNode(lhs) && readNode(rhs);
                break;
            case OR:
                readValue = readNode(lhs) || readNode(rhs);
                break;
            case XOR:
                readValue = readNode(lhs) ^ readNode(rhs);
                break;
            case INIT:
                return value;
        }
        hasValue = true;
        value = readValue;
        return value;
    }

    void reset() {
        if (type == INIT) return;
        hasValue = false;
        resetNode(lhs);
        resetNode(rhs);
    }

    void validate() {
        if (type == INIT) return;
        if (lhs == rhs) {
            DEBUG("Found lhs=rhs");
        }
        validateNode(lhs);
        validateNode(rhs);
    }
};

struct Node {
    std::string name;
    bool hasValue;
    bool value;
    Gate* inGate = nullptr;

    bool read() {
        //if (hasValue) return value;
        bool readValue = inGate->read();
        value = readValue;
        hasValue = true;
        return value;
    }

    void reset() {
        hasValue = false;
        if (inGate == nullptr) {
            DEBUG("WTF, seems like %s has a null pointer for inGate", name.c_str());
        }
        inGate->reset();
    }

    void validate() {
        inGate->validate();
    }
};

void swapOuts(Gate& first, Gate& second) {
    Node* temp = first.out;
    first.out = second.out;
    first.out->inGate = &first;
    second.out = temp;
    second.out->inGate = &second;
}

bool readNode(Node* node) {
    return node->read();
}
void resetNode(Node* node) {
    if (node == nullptr) {
        DEBUG("WTF.. trying to reset null");
    }
    node->reset();
}
void validateNode(Node* node) {
    node->validate();
}

std::regex gatePattern("([a-z0-9]+) (OR|AND|XOR) ([a-z0-9]+) -> ([a-z0-9]+)");

std::map<std::string, Node> parse(std::ifstream& in) {
    std::map<std::string, Node> nodesByName;

    bool readingGates = false;
    int gateId = 0;
    auto lambda = [&](std::string line) {
        if (line.empty()) {
            readingGates = true;
            return true;
        }
        if (readingGates) {
            std::smatch match = *std::sregex_iterator(line.begin(), line.end(), gatePattern);
            std::string lhs = match[1];
            std::string op = match[2];
            std::string rhs = match[3];
            std::string out = match[4];
            DEBUG("%s %s %s -> %s", lhs.c_str(), op.c_str(), rhs.c_str(), out.c_str());

            Node& lhsNode = nodesByName[lhs];
            lhsNode.name = lhs;

            Node& rhsNode = nodesByName[rhs];
            rhsNode.name = rhs;

            Node& outNode = nodesByName[out];
            outNode.name = out;

            Gate* gate = (Gate*) malloc(sizeof(Gate));
            gate->id = gateId++;
            gate->type = gateTypeFromString(op);
            gate->hasValue = false;
            gate-> value = false;
            gate->out = &nodesByName[out];
            gate->lhs = &nodesByName[lhs];
            gate->rhs = &nodesByName[rhs];

            outNode.inGate = gate;
        } else {
            auto parts = tools::split(line, ": ");

            Node& initNode = nodesByName[parts[0]];
            initNode.name = parts[0];

            Gate* gate = (Gate*) malloc(sizeof(Gate));
            gate->id = gateId++;
            gate->type = INIT;
            gate->hasValue = true;
            gate->value = std::stoi(parts[1]) == 1;
            gate->out = &initNode;
            gate->lhs = nullptr;
            gate->rhs = nullptr;

            initNode.inGate = gate;
        }
        return true;
    };
    tools::readLines(in, lambda);

    return nodesByName;
}

void part1(std::ifstream& in) {
    std::map<std::string, Node> nodesByName = parse(in);
    std::vector<std::string> outputNodes;

    for (auto& [nodeName, node] : nodesByName) {
        if (nodeName[0] != 'z') continue;
        outputNodes.push_back(nodeName);
    }
    std::sort(outputNodes.begin(), outputNodes.end());

    unsigned long long value = 0;
    int shiftBy = 0;
    for (auto& outNodeName : outputNodes) {
        unsigned long long nodeValue = nodesByName[outNodeName].read();
        value = value | (nodeValue << shiftBy);
        shiftBy++;
    }

    INFO("Part 1: %lld", value);
}

typedef std::pair<std::string, std::string> SwapPair;

struct Adder {
    std::set<Gate*> gates;
    std::map<std::string, Node> nodesByName;
    std::vector<std::string> outputNodes;

    void printGraphviz(unsigned long long target) {
        std::ostringstream out;

        unsigned long long value = getValue();
        std::map<std::string, bool> correct;
        int i = 0;
        while (value > 0) {
            bool isGood = (value & 1) == (target & 1);
            std::ostringstream name;
            name << "z";
            if (i < 10) {
                name << "0";
            }
            name << i;
            correct[name.str()] = isGood;
            i++;
            value >>= 1;
            target >>= 1;
        }

        out << "digraph G {" << std::endl;
        out << "rank1 [style=transparent];\n";
        out << "rank2 [style=transparent];\n";
        out << "rank1 -> rank2 [color=white];\n";
        out << "{rank=same rank1 ";
        for (int i = 0; i < 45; i++) {
            std::ostringstream name;
            if (i < 10) {
                name << "0";
            }
            name << i;
            out << "-> x" << name.str() << " -> y" << name.str() << " ";
        }
        out <<" [color=white] rankdir=LR}\n;";
        for (int i = 0; i < 46; i++) {
            std::ostringstream name;
            name << "z";
            if (i < 10) {
                name << "0";
            }
            name << i;
            if (!correct[name.str()]) {
                out << " " << name.str() << "[color=red]; ";
            } else {
                out << " " << name.str() << "; ";
            }
        }
        out << "\n{rank=same rank2 ";
        for (int i = 0; i < 46; i++) {
            std::ostringstream name;
            name << "z";
            if (i < 10) {
                name << "0";
            }
            name << i;
            out << "-> " << name.str() << " ";
        }
        out << " [color=white] rankdir=LR}\n";
        for (auto& [name, node] : nodesByName) {
            if (name[0] == 'x' || name[0] == 'y' || name[0] == 'z') continue;
            out << "  " << name << ";" << std::endl;
        }
        for (Gate* gateP : gates) {
            Gate gate = *gateP;
            switch (gate.type) {
                case INIT:
                    out << "  INIT" << gate.id << " [label=\"" << (gate.value ? '1' : '0') << "\"];";
                    out << "  INIT" << gate.id << " -> " << gate.out->name << ";\n";
                    break;
                case AND:
                    out << "  AND" << gate.id << " [label=\"AND\"];";
                    out << "  " << gate.lhs->name << " -> " << "AND" << gate.id << ";";
                    out << "  " << gate.rhs->name << " -> " << "AND" << gate.id << ";";
                    out << "  AND" << gate.id << " -> " << gate.out->name << ";\n";
                    break;
                case OR:
                    out << "  OR" << gate.id << " [label=\"OR\"];";
                    out << "  " << gate.lhs->name << " -> " << "OR" << gate.id << ";";
                    out << "  " << gate.rhs->name << " -> " << "OR" << gate.id << ";";
                    out << "  OR" << gate.id << " -> " << gate.out->name << ";\n";
                    break;
                case XOR:
                    out << "  XOR" << gate.id << " [label=\"XOR\"];";
                    out << "  " << gate.lhs->name << " -> " << "XOR" << gate.id << ";";
                    out << "  " << gate.rhs->name << " -> " << "XOR" << gate.id << ";";
                    out << "  XOR" << gate.id << " -> " << gate.out->name << ";\n";
                    break;
            }
        }
        out << "}";

        printf("%s\n", out.str().c_str());
    }

    void swapWires(const SwapPair& pair) {
        Node& first = nodesByName[pair.first];
        Node& second = nodesByName[pair.second];
        auto temp = first.inGate;
        first.inGate = second.inGate;
        first.inGate->out = &first;
        second.inGate = temp;
        second.inGate->out = &second;
    }

    void reset() {
        for (auto& outNodeName : outputNodes) {
            Node& node = nodesByName[outNodeName];
            node.reset();
        }
    }

    unsigned long long getValue() {
        unsigned long long value = 0;
        int shiftBy = 0;
        for (auto& outNodeName : outputNodes) {
            Node& node = nodesByName[outNodeName];
            unsigned long long nodeValue = node.read();
            value = value | (nodeValue << shiftBy);
            shiftBy++;
        }
        return value;
    }

    void validate() {
        for (auto& outNodeName : outputNodes) {
            Node& node = nodesByName[outNodeName];
            node.validate();
        }
    }
};

std::map<std::set<std::string>, unsigned long long> cache;

std::set<std::string> findSwaps(Adder& adder, std::set<SwapPair>& pairs, unsigned long long target, std::set<std::string>& soFar, int searchSize) {
    if (soFar.size() == searchSize * 2) {
        if (cache.contains(soFar)) {
            if (cache[soFar] == target) {
                return soFar;
            }
            return std::set<std::string>{};
        }
        auto value = adder.getValue();
        cache[soFar] = value;
        if (value == target) {
            return soFar;
        }
        return std::set<std::string>{};
    }
    for (auto& pair : pairs) {
        if (soFar.contains(pair.first) || soFar.contains(pair.second)) continue;
        soFar.insert(pair.first);
        soFar.insert(pair.second);
        adder.swapWires(pair);
        auto res = findSwaps(adder, pairs, target, soFar, searchSize);
        if (!res.empty()) {
            return res;
        }
        adder.swapWires(pair);
        soFar.erase(pair.first);
        soFar.erase(pair.second);
    }
    return std::set<std::string>{};
}

void part2(std::ifstream& in) {
    std::set<Gate*> gates;
    std::map<std::string, Node> nodesByName = parse(in);
    std::vector<std::string> nodes;
    std::vector<std::string> xNodes;
    std::vector<std::string> yNodes;
    std::vector<std::string> outputNodes;

    for (auto& [nodeName, node] : nodesByName) {
        if (nodeName[0] == 'x') xNodes.push_back(nodeName);
        if (nodeName[0] == 'y') yNodes.push_back(nodeName);
        if (nodeName[0] == 'z') outputNodes.push_back(nodeName);
        gates.insert(node.inGate);
        nodes.push_back(nodeName);
    }
    std::sort(xNodes.begin(), xNodes.end());
    std::sort(yNodes.begin(), yNodes.end());
    std::sort(outputNodes.begin(), outputNodes.end());

    unsigned long long xValue;
    unsigned long long yValue;
    for (int i = xNodes.size() - 1; i >= 0; i--) {
        xValue = xValue | nodesByName[xNodes[i]].read();
        yValue = yValue | nodesByName[yNodes[i]].read();
        if (i != 0) {
            xValue <<= 1;
            yValue <<= 1;
        }
    }
    unsigned long long targetValue = xValue + yValue;

    std::set<SwapPair> allPairs;
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i][0] == 'x' || nodes[i][0] == 'y') continue;
        for (int j = 0; j < nodes.size(); j++) {
            if (i == j) continue;
            if (nodes[j][0] == 'x' || nodes[j][0] == 'y') continue;

            std::string first = nodes[i] < nodes[j] ? nodes[i] : nodes[j];
            std::string second = nodes[i] < nodes[j] ? nodes[j] : nodes[i];
            allPairs.insert(SwapPair{first, second});
        }
    }

    std::set<std::string> empty{};
    Adder adder{
        .gates = gates,
        .nodesByName = nodesByName,
        .outputNodes = outputNodes,
    };
    adder.printGraphviz(targetValue);
    auto x = *allPairs.begin();
    adder.swapWires(x);
    adder.swapWires(x);
    adder.validate();
    adder.reset();

    INFO("%lld == %lld (%d)", adder.getValue(), targetValue, adder.getValue() == targetValue);

    /*
    //std::set<std::string> answer = findSwaps(adder, allPairs, targetValue, empty, 2);

    std::ostringstream result;
    bool isFirst = true;
    for (auto node : answer) {
        if (!isFirst) {
            result << ",";
        }
        result << node;
        isFirst = false;
    }

    INFO("Part 2: %s", result.str().c_str());
    */
}

MAIN(part1, part2);