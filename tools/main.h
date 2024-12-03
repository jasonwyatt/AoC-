#ifndef _TOOLS_MAIN_H
#define _TOOLS_MAIN_H

// Builds a main function that passes a @c std::ifstream to the @c part1Fn and @c part2Fn functions.
// The @c testpath string literal serves as a default input file location.
#define MAIN(defaultTestPath, part1Fn, part2Fn) \
    int main(int argc, char** argv) { \
        const char* fileName; \
        if (argc > 1) { \
            fileName = argv[1]; \
        } else { \
            fileName = defaultTestPath; \
        } \
        \
        { \
            std::ifstream file(fileName); \
            if (!file.is_open()) { \
                std::cerr << "Oops, file " << fileName << " cant be opened." << std::endl; \
                return -1; \
            } \
            part1Fn(file); \
            file.close(); \
        } \
        { \
            std::ifstream file(fileName); \
            if (!file.is_open()) { \
                std::cerr << "Oops, file " << fileName << " cant be opened." << std::endl; \
                return -1; \
            } \
            part2Fn(file); \
            file.close(); \
        } \
        return 0; \
    }

#endif