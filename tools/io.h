#ifndef _TOOLS_IO_H
#define _TOOLS_IO_H

#include <fstream>
#include <vector>

namespace tools {

typedef std::vector<std::vector<char>> CharGrid;
typedef std::vector<std::vector<int>> IntGrid;

CharGrid readCharGrid(std::ifstream& in);

IntGrid readIntGrid(std::ifstream& in);

}

#endif