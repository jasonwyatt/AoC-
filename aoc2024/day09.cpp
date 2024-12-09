#include <fstream>
#include <iostream>
#include <vector>
#include <set>

#include "tools/log.h"
#include "tools/main.h"

void part1(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line)) {
        ERROR("Couldn't read line");
        return;
    }

    int totalLen = 0;
    int* digits = (int*) calloc(line.size(), sizeof(int));
    for (int i = 0; i < line.size(); i++) {
        digits[i] = line[i] - '0';
        if (i % 2 == 0) {
            totalLen += digits[i];
        }
    }

    int* result = (int*) calloc(totalLen, sizeof(int));

    int writeHead = 0;
    int headReadHead = 0;
    int tailReadHead = line.size() - 1;

    while (writeHead < totalLen) {
        if (headReadHead % 2 == 0) {
            // non-gap
            for (int i = 0; i < digits[headReadHead]; i++) {
                result[writeHead++] = headReadHead / 2;
            }
            headReadHead++;
        } else {
            // gap..
            int gapSize = digits[headReadHead];
            int i = 0;
            for (i = 0; i < gapSize && i < digits[tailReadHead]; i++) {
                result[writeHead++] = tailReadHead / 2;
            }
            if (i < gapSize) {
                digits[headReadHead] = gapSize - i;
            } else {
                headReadHead++;
            }
            if (i < digits[tailReadHead]) {
                digits[tailReadHead] = digits[tailReadHead] - i;
            } else {
                tailReadHead -= 2;
            }
        }
    }

    long checkSum = 0;
    for (int i = 0; i < totalLen; i++) {
        checkSum += (i * result[i]);
    }
    INFO("Part 1: %ld", checkSum);
}

typedef struct {
    bool isGap;
    int size;
    int fileId;
} Chunk;

void part2(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line)) {
        ERROR("Couldn't read line");
        return;
    }

    std::vector<Chunk> chunks;
    std::vector<Chunk>& latest = chunks;

    for (int i = 0; i < line.size(); i++) {
        chunks.push_back(
            {
                .isGap =  i % 2 == 1,
                .size = line[i] - '0',
                .fileId = i / 2
            }
        );
    }

    std::set<int> attempted;
    int currentCandidateIndex = chunks.size() - 1;
    do {
        Chunk current = latest[currentCandidateIndex];

        std::vector<Chunk> moved;
        int i = 0;
        int destinationGap = -1;
        while (i < latest.size() && i < currentCandidateIndex) {
            if (latest[i].isGap && latest[i].size >= current.size) {
                destinationGap = i;
                break;
            }
            i++;
        }
        i = 0;
        while (i < latest.size()) {
            if (i == destinationGap) {
                moved.push_back(current);
                moved.push_back({.isGap = true, .size = latest[i].size - current.size, .fileId = -1});
            } else if (latest[i].fileId != current.fileId || latest[i].isGap != current.isGap || destinationGap == -1) {
                moved.push_back(latest[i]);
            } else if (i == currentCandidateIndex && destinationGap != -1) {
                moved.push_back({.isGap = true, .size = current.size, .fileId = -1});
            }
            i++;
        }

        std::vector<Chunk> compacted;
        int gapSize = 0;
        for (Chunk c : moved) {
            if (c.isGap) {
                gapSize += c.size;
            } else {
                if (gapSize > 0) {
                    compacted.push_back({ .isGap = true, .size = gapSize, .fileId = -1 });
                    gapSize = 0;
                }
                compacted.push_back(c);
            }
        }
        latest = compacted;

        attempted.insert(current.fileId);
        currentCandidateIndex = -1;
        for (int i = latest.size() - 1; i >= 0; i--) {
            if (latest[i].isGap == false && !attempted.contains(latest[i].fileId)) {
                currentCandidateIndex = i;
                break;
            }
        }
    } while (currentCandidateIndex > 0);

    long checksum = 0;
    int i = 0;
    for (Chunk chunk : latest) {
        for (int j = 0; j < chunk.size; j++) {
            if (!chunk.isGap) {
                checksum += (i + j) * chunk.fileId;
            }
        }
        i += chunk.size;
    }
    INFO("Part 2: %ld", checksum);
}

MAIN(part1, part2);