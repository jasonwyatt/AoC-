#include "tools/log.h"

int logging::Level::kLogLevel = DEBUG_LEVEL;

bool logging::isEnabled(LogLevelValue value) {
    return logging::Level::kLogLevel <= value;
}
