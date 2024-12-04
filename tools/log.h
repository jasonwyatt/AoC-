#ifndef _TOOLS_LOG_H
#define _TOOLS_LOG_H

#include <cstdio>

typedef int LogLevelValue;

#define DEBUG_LEVEL LogLevelValue(0)
#define INFO_LEVEL LogLevelValue(1)
#define WARN_LEVEL LogLevelValue(2)
#define ERROR_LEVEL LogLevelValue(3)

namespace logging {

/*
Name            FG  BG
Black           30  40
Red             31  41
Green           32  42
Yellow          33  43
Blue            34  44
Magenta         35  45
Cyan            36  46
White           37  47
Bright Black    90  100
Bright Red      91  101
Bright Green    92  102
Bright Yellow   93  103
Bright Blue     94  104
Bright Magenta  95  105
Bright Cyan     96  106
Bright White    97  107
*/

class Level {
public:
    static int kLogLevel;
};

bool isEnabled(LogLevelValue value);

}

#define LOG_LEVEL(level) (logging::Level::kLogLevel = level)

#define DEBUG(str, ...) \
    do { \
        if (logging::isEnabled(DEBUG_LEVEL)) { \
            fprintf(stdout, "\x1B[34mDEBUG: " str "\033[0m\n", ##__VA_ARGS__); \
        } \
    } while(0)

#define INFO(str, ...) \
    do { \
        if (logging::isEnabled(INFO_LEVEL)) { \
            fprintf(stderr, "INFO:  " str "\n", ##__VA_ARGS__); \
        } \
    } while(0)

#define WARN(str, ...) \
    do { \
        if (logging::isEnabled(WARN_LEVEL)) { \
            fprintf(stderr, "\x1B[33mWARN:  " str "\033[0m\n", ##__VA_ARGS__); \
        } \
    } while(0)

#define ERROR(str, ...) \
    do { \
        if (logging::isEnabled(ERROR_LEVEL)) { \
            fprintf(stderr, "\x1B[31mERROR: " str "\033[0m\n", ##__VA_ARGS__); \
        } \
    } while(0)

#endif