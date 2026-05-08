#pragma once

#include <Arduino.h>
#include <stdarg.h>

#include "led_driver.h"
#include "utils/log_tags.h"

enum LogLevel {
    LOG_ERROR = 0,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
};

extern LedDriver statusLed;

class Logger {
public:

    static void begin(unsigned long baud = 115200);

    static void setLevel(LogLevel level);

    static void error(const char* tag, const char* format, ...);
    static void warn(const char* tag, const char* format, ...);
    static void info(const char* tag, const char* format, ...);
    static void debug(const char* tag, const char* format, ...);

private:

    static LogLevel currentLevel;

    static void log(
        LogLevel level,
        const char* levelStr,
        const char* tag,
        const char* format,
        va_list args
    );

    static String getTimestamp();
};