#ifndef _TOOLS_MAIN_H
#define _TOOLS_MAIN_H

#include <fstream>
#include <functional>

namespace tools {
    int _mainImpl(int argc,
                  char** argv,
                  const char* defaultTestPath,
                  std::function<void(std::ifstream&)> part1,
                  std::function<void(std::ifstream&)> part2);
}

// Builds a main function that passes a @c std::ifstream to the @c part1Fn and @c part2Fn functions.
// The @c testpath string literal serves as a default input file location.
#define MAIN_WITH_DEFAULT(defaultTestPath, part1Fn, part2Fn) \
    int main(int argc, char** argv) { \
        return tools::_mainImpl(argc, argv, defaultTestPath, \
                                [](std::ifstream& in){ part1Fn(in); }, \
                                [](std::ifstream& in){ part2Fn(in); }); \
    }

// Builds a main function that passes a @c std::ifstream to the @c part1Fn and @c part2Fn functions.
#define MAIN(part1Fn, part2Fn) \
    MAIN_WITH_DEFAULT("NO DEFAULT AVAILABLE", part1Fn, part2Fn)

#endif