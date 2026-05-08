#include "logger.h"
#include "config/pins.h"

LogLevel Logger::currentLevel = LOG_DEBUG;

LedDriver statusLed(STATUS_LED_PIN, true);

void Logger::begin(unsigned long baud) {

    Serial.begin(baud);

    while (!Serial) {
        delay(10);
    }

    statusLed.begin();
    Serial.println();
    Serial.println("===== LOGGER STARTED =====");
}

void Logger::setLevel(LogLevel level) {
    currentLevel = level;
}

void Logger::error(const char* tag, const char* format, ...) {

    va_list args;
    va_start(args, format);

    log(LOG_ERROR, "ERROR", tag, format, args);

    va_end(args);
}

void Logger::warn(const char* tag, const char* format, ...) {

    va_list args;
    va_start(args, format);

    log(LOG_WARN, "WARN", tag, format, args);

    va_end(args);
}

void Logger::info(const char* tag, const char* format, ...) {

    va_list args;
    va_start(args, format);

    log(LOG_INFO, "INFO", tag, format, args);

    va_end(args);
}

void Logger::debug(const char* tag, const char* format, ...) {

    va_list args;
    va_start(args, format);

    log(LOG_DEBUG, "DEBUG", tag, format, args);

    va_end(args);
}

void Logger::log(
    LogLevel level,
    const char* levelStr,
    const char* tag,
    const char* format,
    va_list args
) {

    if (level > currentLevel) {
        return;
    }

    char messageBuffer[256];

    vsnprintf(
        messageBuffer,
        sizeof(messageBuffer),
        format,
        args
    );

    Serial.printf(
        "[%s] [%s] [%s] %s\n",
        getTimestamp().c_str(),
        levelStr,
        tag,
        messageBuffer
    );
}

String Logger::getTimestamp() {

    unsigned long seconds = millis() / 1000;

    unsigned long hrs = seconds / 3600;
    unsigned long mins = (seconds % 3600) / 60;
    unsigned long secs = seconds % 60;

    char buffer[16];

    snprintf(
        buffer,
        sizeof(buffer),
        "%02lu:%02lu:%02lu",
        hrs,
        mins,
        secs
    );

    return String(buffer);
}