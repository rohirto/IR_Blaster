#pragma once

#include <ArduinoJson.h>

#include "logger.h"
#include "config/system_config.h"

class JsonUtils {
public:

    static bool deserialize(
        JsonDocument& doc,
        const String& json,
        const char* tag
    );

    static bool validateSize(
        const String& json,
        const char* tag
    );

    static bool deserializeWithSizeValidation(
        JsonDocument& doc,
        const String& json,
        const char* tag
    );

    static String serialize(
        const JsonDocument& doc,
        const char* tag
    );

    static const size_t MAX_JSON_DOC_SIZE = 1024;
    
};