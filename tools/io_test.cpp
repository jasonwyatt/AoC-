#include "tools/io.h"

#include <vector>
#include <strstream>

#include "gtest/gtest.h"

using namespace tools;

TEST(IO, readCharGrid) {
    std::string input =
        "abcd\n"
        "efgh\n"
        "ijkl\n"
        "mnop\n";

    CharGrid expected = {
        { 'a', 'b', 'c', 'd' },
        { 'e', 'f', 'g', 'h' },
        { 'i', 'j', 'k', 'l' },
        { 'm', 'n', 'o', 'p' },
    };

    std::istringstream actual(input);
    EXPECT_EQ(expected, readCharGrid(actual));
}

TEST(IO, readIntGrid) {
    std::string input =
        "012345\n"
        "678901\n";

    IntGrid expected = {
        { 0, 1, 2, 3, 4, 5 },
        { 6, 7, 8, 9, 0, 1 },
    };

    std::istringstream actual(input);
    EXPECT_EQ(expected, readIntGrid(actual));
}

TEST(IO, readLines) {
    std::string input =
        "this is line 1\n"
        "this is line 2\n"
        "end";

    std::vector<std::string> expected = {
        "this is line 1",
        "this is line 2",
        "end"
    };

    std::istringstream actual(input);
    EXPECT_EQ(expected, readLines(actual));
}