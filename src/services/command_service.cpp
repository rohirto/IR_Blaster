#include "services/command_service.h"

#include <ArduinoJson.h>

#include "utils/logger.h"
#include "utils/json_utils.h"
#include "config/constants.h"
#include "config/system_config.h"
#include "services/storage_service.h"

String CommandService::getCommandFilePath(
    const String& deviceId
) {

    return COMMAND_FILE_PREFIX + deviceId + ".json";
}

bool CommandService::saveCommand(
    const String& deviceId,
    const IRCommand& command
) {

    String path =
        getCommandFilePath(deviceId);

    JsonDocument doc;

    // Load existing commands if file exists
    if (StorageService::exists(path)) {

        String existingJson =
            StorageService::readFile(path);

        // =================================
        // Validate Size and deserialize
        // =================================

        if (!JsonUtils::deserializeWithSizeValidation(doc, existingJson, TAG_COMMAND)) {
            return false;
        }

    }

    JsonObject root;

    if (doc.is<JsonObject>())
    {

        root = doc.as<JsonObject>();
    }
    else
    {

        root = doc.to<JsonObject>();
    }

    if (
        !root[command.action].is<JsonObject>() &&
        root.size() >= MAX_ACTIONS_PER_DEVICE)
    {

        Logger::warn(
            TAG_COMMAND,
            "Maximum actions limit reached");

        return false;
    }

    JsonObject cmd =
        root[command.action]
            .to<JsonObject>();

    cmd["protocol"] =
        command.protocol;

    cmd["data"] =
        command.data;

    cmd["bits"] =
        command.bits;

    String output;

    serializeJsonPretty(
        doc,
        output
    );

    Logger::debug(
        TAG_COMMAND,
        "Free heap: %u",
        ESP.getFreeHeap()
    );

    Logger::debug(
        TAG_COMMAND,
        "Writing JSON: %s",
        output.c_str()
    );

    bool result =
        StorageService::writeFile(
            path,
            output
        );

    if (result) {

        Logger::info(
            TAG_COMMAND,
            "Saved command: %s -> %s",
            deviceId.c_str(),
            command.action.c_str()
        );
    }

    return result;
}

bool CommandService::hasCommand(
    const String& deviceId,
    const String& action
) {

    String path =
        getCommandFilePath(deviceId);

    if (!StorageService::exists(path)) {

        return false;
    }

    String json =
        StorageService::readFile(path);
    // =================================
    // Validate Size and deserialize
    // =================================

    JsonDocument doc;
    if (!JsonUtils::deserializeWithSizeValidation(doc, json, TAG_COMMAND)) {
        return false;
    }

    return doc[action].is<JsonObject>();
}

IRCommand CommandService::getCommand(
    const String& deviceId,
    const String& action
) {

    String path =
        getCommandFilePath(deviceId);

    if (!StorageService::exists(path)) {

        return IRCommand();
    }

    String json =
        StorageService::readFile(path);
    
    // Validate Size and deserialize
    JsonDocument doc;
    if (!JsonUtils::deserializeWithSizeValidation(doc, json, TAG_COMMAND)) {
        return IRCommand();
    }

    if (!doc[action].is<JsonObject>()) {

        return IRCommand();
    }

    JsonObject obj =
        doc[action];

    return IRCommand(
        action,
        obj["protocol"].as<String>(),
        obj["data"].as<String>(),
        obj["bits"].as<uint16_t>()
    );
}

std::vector<IRCommand> CommandService::getCommands(
    const String& deviceId
) {

    std::vector<IRCommand> commands;

    String path =
        getCommandFilePath(deviceId);

    if (!StorageService::exists(path)) {

        return commands;
    }

    String json =
        StorageService::readFile(path);
    // Validate Size and deserialize
    JsonDocument doc;
    if (!JsonUtils::deserializeWithSizeValidation(doc, json, TAG_COMMAND)) {
        return commands;
    }
    JsonObject root =
        doc.as<JsonObject>();

    for (JsonPair kv : root) {

        String action =
            kv.key().c_str();

        JsonObject obj =
            kv.value().as<JsonObject>();

        commands.push_back(
            IRCommand(
                action,
                obj["protocol"].as<String>(),
                obj["data"].as<String>(),
                obj["bits"].as<uint16_t>()
            )
        );
    }

    return commands;
}

bool CommandService::deleteCommands(
    const String& deviceId
) {

    String path =
        getCommandFilePath(deviceId);

    if (!StorageService::exists(path)) {

        return true;
    }

    bool result =
        StorageService::deleteFile(path);

    if (result) {

        Logger::info(
            TAG_COMMAND,
            "Deleted commands for device: %s",
            deviceId.c_str()
        );
    }

    return result;
}

bool CommandService::deleteCommand(
    const String& deviceId,
    const String& action
) {

    String path =
        getCommandFilePath(deviceId);

    if (!StorageService::exists(path)) {

        Logger::warn(
            TAG_COMMAND,
            "Command file not found"
        );

        return false;
    }

    String json =
        StorageService::readFile(path);
    // =================================
    // Validate Size and deserialize
    // =================================

    JsonDocument doc;
    if (!JsonUtils::deserializeWithSizeValidation(doc, json, TAG_COMMAND)) {
        return false;
    }

    JsonObject root =
        doc.as<JsonObject>();

    if (!root[action].is<JsonObject>()) {

        Logger::warn(
            TAG_COMMAND,
            "Action not found: %s",
            action.c_str()
        );

        return false;
    }

    root.remove(action);

    String output;

    serializeJsonPretty(
        doc,
        output
    );

    Logger::debug(
        TAG_COMMAND,
        "Updated JSON: %s",
        output.c_str()
    );

    bool result =
        StorageService::writeFile(
            path,
            output
        );

    if (result) {

        Logger::info(
            TAG_COMMAND,
            "Deleted action '%s' for device '%s'",
            action.c_str(),
            deviceId.c_str()
        );
    }

    return result;
}