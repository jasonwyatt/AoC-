#ifndef _TOOLS_IO_H
#define _TOOLS_IO_H

#include <istream>
#include <functional>
#include <vector>

#include "absl/strings/str_split.h"

namespace tools {

/**
 * Reads lines from @c in and calls the @c lineHandler callback with them. The callback returns
 * @c true to continue reading from @c in or @c false to stop reading from @in.
 *
 * @typename Func shuld accept a @c std::string and return @c bool.
 */
template <typename Func>
int readLines(std::istream& in, Func& lineHandler) {
    std::string line;
    int i = 0;
    while (std::getline(in, line) && lineHandler(line)) {
        i++;
    }
    return i;
}
std::vector<std::string> readLines(std::istream& in);

template <typename Item, typename Converter>
std::vector<Item> interpretLines(std::istream& in, Converter& converter) {
    std::vector<Item> result;
    auto lambda = [&](std::string line) {
        result.push_back(converter(line));
        return true;
    };
    readLines(in, lambda);
    return result;
}

typedef std::vector<std::vector<char>> CharGrid;
CharGrid readCharGrid(std::istream &in);

typedef std::vector<std::vector<int>> IntGrid;
IntGrid readIntGrid(std::istream &in);

std::vector<std::string> split(std::string str, std::string delim);

}

#endif