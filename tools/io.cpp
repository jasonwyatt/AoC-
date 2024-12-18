#include "tools/io.h"

#include <istream>
#include <iostream>
#include <vector>

#include "absl/strings/str_split.h"

std::vector<std::string> tools::readLines(std::istream& in) {
    auto lambda = [](std::string line) {
        return line;
    };
    return tools::interpretLines<std::string>(in, lambda);
}

tools::CharGrid tools::readCharGrid(std::istream &in) {
  auto lambda = [](std::string line) {
    std::vector<char> chars;
    for (char c : line) {
      chars.push_back(c);
    }
    return chars;
  };
  return tools::interpretLines<std::vector<char>>(in, lambda);
}

tools::IntGrid tools::readIntGrid(std::istream &in) {
  auto lambda = [](std::string line) {
    std::vector<int> ints;
    for (char c : line) {
      ints.push_back(c - '0');
    }
    return ints;
  };
  return tools::interpretLines<std::vector<int>>(in, lambda);
}

std::vector<std::string> tools::split(std::string str, std::string delim) {
  std::vector<std::string> split = absl::StrSplit(str, delim);
  return split;
}
