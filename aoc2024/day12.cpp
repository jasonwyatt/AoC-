#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "tools/log.h"
#include "tools/main.h"

typedef std::vector<std::string> Map;

void traverse(std::map<std::pair<int, int>, int>& regionsByCoordinate, Map& map, int region, int i, int j, char expectedCrop) {
    if (i < 0 || i > map.size() || j < 0 || j > map[0].size()) return;
    if (map[i][j] != expectedCrop) return;

    std::pair<int, int> coord(i, j);
    if (regionsByCoordinate.contains(coord)) return;
    regionsByCoordinate[coord] = region;

    traverse(regionsByCoordinate, map, region, i - 1, j, expectedCrop);
    traverse(regionsByCoordinate, map, region, i + 1, j, expectedCrop);
    traverse(regionsByCoordinate, map, region, i, j - 1, expectedCrop);
    traverse(regionsByCoordinate, map, region, i, j + 1, expectedCrop);
}

struct SideCountingContext {
    std::map<std::pair<int, int>, int>& regionsByCoordinate;
    std::map<int, std::vector<std::pair<int, int>>>& coordinatesByRegion;
    std::map<std::pair<int, int>, int> verticalLeftSidesByCoordinate; // left side is the vertical edge
    std::map<std::pair<int, int>, int> verticalRightSidesByCoordinate; // right side is the vertical edge
    std::map<std::pair<int, int>, int> horizontalTopSidesByCoordinate; // top side is the vertical edge
    std::map<std::pair<int, int>, int> horizontalBottomSidesByCoordinate; // bottom side is the vertical edge
};

bool walkVerticalLeft(SideCountingContext& context, std::pair<int, int> coord, int verticalLeftSideId) {
    if (context.verticalLeftSidesByCoordinate.contains(coord)) return false;
    int myRegion = context.regionsByCoordinate[coord];

    std::pair<int, int> left(coord.first, coord.second - 1);
    if (!context.regionsByCoordinate.contains(left) || context.regionsByCoordinate[left] != myRegion) {
        context.verticalLeftSidesByCoordinate[coord]++;
    } else {
        return false;
    }


    std::pair<int, int> up(coord.first - 1, coord.second);
    if (context.regionsByCoordinate.contains(up) && context.regionsByCoordinate[up] == myRegion) {
        walkVerticalLeft(context, up, verticalLeftSideId);
    }
    std::pair<int, int> down(coord.first + 1, coord.second);
    if (context.regionsByCoordinate.contains(down) && context.regionsByCoordinate[down] == myRegion) {
        walkVerticalLeft(context, down, verticalLeftSideId);
    }
    return true;
}

bool walkVerticalRight(SideCountingContext& context, std::pair<int, int> coord, int verticalRightSideId) {
    if (context.verticalRightSidesByCoordinate.contains(coord)) return false;
    int myRegion = context.regionsByCoordinate[coord];

    std::pair<int, int> right(coord.first, coord.second + 1);
    if (!context.regionsByCoordinate.contains(right) || context.regionsByCoordinate[right] != myRegion) {
        context.verticalRightSidesByCoordinate[coord]++;
    } else {
        return false;
    }


    std::pair<int, int> up(coord.first - 1, coord.second);
    if (context.regionsByCoordinate.contains(up) && context.regionsByCoordinate[up] == myRegion) {
        walkVerticalRight(context, up, verticalRightSideId);
    }
    std::pair<int, int> down(coord.first + 1, coord.second);
    if (context.regionsByCoordinate.contains(down) && context.regionsByCoordinate[down] == myRegion) {
        walkVerticalRight(context, down, verticalRightSideId);
    }
    return true;
}

bool walkHorizontalTop(SideCountingContext& context, std::pair<int, int> coord, int horizontalTopSideId) {
    if (context.horizontalTopSidesByCoordinate.contains(coord)) return false;
    int myRegion = context.regionsByCoordinate[coord];

    std::pair<int, int> up(coord.first - 1, coord.second);
    if (!context.regionsByCoordinate.contains(up) || context.regionsByCoordinate[up] != myRegion) {
        context.horizontalTopSidesByCoordinate[coord]++;
    } else {
        return false;
    }


    std::pair<int, int> left(coord.first, coord.second - 1);
    if (context.regionsByCoordinate.contains(left) && context.regionsByCoordinate[left] == myRegion) {
        walkHorizontalTop(context, left, horizontalTopSideId);
    }
    std::pair<int, int> right(coord.first, coord.second + 1);
    if (context.regionsByCoordinate.contains(right) && context.regionsByCoordinate[right] == myRegion) {
        walkHorizontalTop(context, right, horizontalTopSideId);
    }
    return true;
}

bool walkHorizontalBottom(SideCountingContext& context, std::pair<int, int> coord, int horizontalBottomSideId) {
    if (context.horizontalBottomSidesByCoordinate.contains(coord)) return false;
    int myRegion = context.regionsByCoordinate[coord];

    std::pair<int, int> down(coord.first + 1, coord.second);
    if (!context.regionsByCoordinate.contains(down) || context.regionsByCoordinate[down] != myRegion) {
        context.horizontalBottomSidesByCoordinate[coord]++;
    } else {
        return false;
    }


    std::pair<int, int> left(coord.first, coord.second - 1);
    if (context.regionsByCoordinate.contains(left) && context.regionsByCoordinate[left] == myRegion) {
        walkHorizontalBottom(context, left, horizontalBottomSideId);
    }
    std::pair<int, int> right(coord.first, coord.second + 1);
    if (context.regionsByCoordinate.contains(right) && context.regionsByCoordinate[right] == myRegion) {
        walkHorizontalBottom(context, right, horizontalBottomSideId);
    }
    return true;
}

void part1(std::ifstream& in) {
    Map map;
    std::string line;
    while (getline(in, line)) {
        map.push_back(std::string(line));
    }

    std::map<std::pair<int, int>, int> regionsByCoordinate;
    int currentRegion = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            std::pair<int, int> coord(i, j);
            if (!regionsByCoordinate.contains(coord)) {
                traverse(regionsByCoordinate, map, currentRegion, i, j, map[i][j]);
                currentRegion++;
            }
        }
    }

    std::map<int, std::vector<std::pair<int, int>>> reversed;
    for (auto [coord, region] : regionsByCoordinate) {
        if (!reversed.contains(region)) {
            reversed[region] = { coord };
        } else {
            reversed[region].push_back(coord);
        }
    }

    int total = 0;
    for (auto [region, coords] : reversed) {
        int area = coords.size();
        int perimeter = 0;
        for (auto coord : coords) {
            std::pair<int, int> up(coord.first - 1, coord.second);
            std::pair<int, int> down(coord.first + 1, coord.second);
            std::pair<int, int> left(coord.first, coord.second - 1);
            std::pair<int, int> right(coord.first, coord.second + 1);
            auto neighbors = { up, down, left, right };
            for (auto neighbor : neighbors) {
                if (regionsByCoordinate.contains(neighbor)) {
                    if (regionsByCoordinate[neighbor] != region) {
                        perimeter++;
                    }
                } else {
                    perimeter++;
                }
            }
        }
        total += area * perimeter;
    }
    INFO("Part 1: %d", total);
}

void part2(std::ifstream& in) {
    Map map;
    std::string line;
    while (getline(in, line)) {
        map.push_back(std::string(line));
    }

    std::map<std::pair<int, int>, int> regionsByCoordinate;
    int currentRegion = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            std::pair<int, int> coord(i, j);
            if (!regionsByCoordinate.contains(coord)) {
                traverse(regionsByCoordinate, map, currentRegion, i, j, map[i][j]);
                currentRegion++;
            }
        }
    }

    std::map<int, std::vector<std::pair<int, int>>> reversed;
    for (auto [coord, region] : regionsByCoordinate) {
        if (!reversed.contains(region)) {
            reversed[region] = { coord };
        } else {
            reversed[region].push_back(coord);
        }
    }

    int total = 0;
    SideCountingContext context{
        .regionsByCoordinate = regionsByCoordinate,
        .coordinatesByRegion = reversed,
    };
    for (auto [region, coords] : reversed) {
        int area = coords.size();
        int verticalLeftEdge = 0;
        int verticalRightEdge = 0;
        int horizontalTopEdge = 0;
        int horizontalBottomEdge = 0;
        for (auto coord : coords) {
            if (walkVerticalLeft(context, coord, verticalLeftEdge)) {
                verticalLeftEdge++;
            }
            if (walkVerticalRight(context, coord, verticalRightEdge)) {
                verticalRightEdge++;
            }
            if (walkHorizontalBottom(context, coord, horizontalBottomEdge)) {
                horizontalBottomEdge++;
            }
            if (walkHorizontalTop(context, coord, horizontalTopEdge)) {
                horizontalTopEdge++;
            }
        }
        total += area * (verticalLeftEdge + verticalRightEdge + horizontalBottomEdge + horizontalTopEdge);
    }
    INFO("Part 2: %d", total);
}

MAIN(part1, part2);