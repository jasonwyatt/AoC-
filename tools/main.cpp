#include <fstream>
#include <functional>

#include "tools/log.h"
#include "tools/main.h"

int tools::_mainImpl(int argc,
                     char** argv,
                     const char* defaultTestPath,
                     std::function<void (std::ifstream &)> part1,
                     std::function<void (std::ifstream &)> part2) {
    const char* fileName;
    if (argc > 1) { \
        fileName = argv[1];
    } else { \
        WARN("No file specified, using default \"%s\"", defaultTestPath);
        fileName = defaultTestPath;
    }
    {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            ERROR("File \"%s\" can't be opened.", fileName);
            return -1;
        }
        part1(file);
        file.close();
    }
    {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            ERROR("File \"%s\" can't be opened.", fileName);
            return -1;
        }
        part2(file);
        file.close();
    }
    return 0;
}