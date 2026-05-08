#include "utils/json_utils.h"

bool JsonUtils::deserialize(
    JsonDocument &doc,
    const String &json,
    const char *tag)
{
    Logger::debug(
            TAG_MEMORY,
            "Heap before deserialize: %u",
            ESP.getFreeHeap());
    DeserializationError error =
        deserializeJson(doc, json);
    Logger::debug(
            TAG_MEMORY,
            "Heap after deserialize: %u",
            ESP.getFreeHeap());

    if (error)
    {

        Logger::error(
            tag,
            "JSON parse failed: %s",
            error.c_str());

        return false;
    }

    return true;
}

bool JsonUtils::validateSize(
    const String &json,
    const char *tag)
{

    if (json.length() > MAX_JSON_DOC_SIZE)
    {

        Logger::error(
            tag,
            "JSON payload exceeds max size");

        return false;
    }

    return true;
}

bool JsonUtils::deserializeWithSizeValidation(
    JsonDocument &doc,
    const String &json,
    const char *tag)
{

    if (!validateSize(json, tag))
    {
        return false;
    }
    
    return deserialize(doc, json, tag);
}

String JsonUtils::serialize(
    const JsonDocument &doc,
    const char *tag)
{

    String json;

    serializeJson(doc, json);

    Logger::debug(
        tag,
        "Serialized JSON: %s",
        json.c_str());

    return json;
}
