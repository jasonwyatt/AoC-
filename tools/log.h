#ifndef _TOOLS_LOG_H
#define _TOOLS_LOG_H

#include <cstdio>

#define DEBUG_LEVEL 0
#define WARN_LEVEL 1
#define ERROR_LEVEL 2

namespace logging {

class Level {
public:
    static int kLogLevel;
};

}

#define LOG_LEVEL(level) (logging::kLogLevel = level)

#define DEBUG(str, ...) \
    do { \
        if (logging::Level::kLogLevel <= DEBUG_LEVEL) fprintf(stdout, "DEBUG: " str "\n", ##__VA_ARGS__); \
    } while(0)

#define WARN(str, ...) \
    do { \
        if (logging::Level::kLogLevel <= WARN_LEVEL) fprintf(stderr, "WARN:  " str "\n", ##__VA_ARGS__); \
    } while(0)

#define ERROR(str, ...) \
    do { \
        if (logging::Level::kLogLevel <= ERROR_LEVEL) fprintf(stderr, "ERROR: " str "\n", ##__VA_ARGS__); \
    } while(0)

#endif