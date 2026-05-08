#include "services/ac/ac_state_service.h"

#include <ArduinoJson.h>

#include "LittleFS.h"

#include "utils/logger.h"

#include "services/storage_service.h"

#include "utils/json_utils.h"

String ACStateService::getStateFilePath(
    const String& deviceId
) {

    return "/ac_state_" +
           deviceId +
           ".json";
}

bool ACStateService::saveState(
    const String& deviceId,
    const ACState& state
) {

    JsonDocument doc;

    doc["power"] =
        state.power;

    doc["temperature"] =
        state.temperature;

    doc["mode"] =
        state.modeToString();

    doc["fan"] =
        state.fanToString();

    doc["swing"] =
        state.swing;

    String output;

    serializeJsonPretty(
        doc,
        output
    );

    Logger::debug(
        "AC_STATE",
        "Saving state: %s",
        output.c_str()
    );

    return StorageService::writeFile(
        getStateFilePath(deviceId),
        output
    );
}

ACState ACStateService::loadState(
    const String& deviceId
) {

    ACState state;

    String path =
        getStateFilePath(deviceId);

    if (!StorageService::exists(path)) {

        Logger::warn(
            "AC_STATE",
            "State file not found, using defaults"
        );

        return state;
    }

    String json =
        StorageService::readFile(path);

    JsonDocument doc;

    if (
        !JsonUtils::deserialize(
            doc,
            json,
            "AC_STATE"
        )
    ) {

        return state;
    }

    state.power =
        doc["power"] | false;

    state.temperature =
        doc["temperature"] | 24;

    state.mode =
        ACState::stringToMode(
            doc["mode"] | "cool"
        );

    state.fan =
        ACState::stringToFan(
            doc["fan"] | "auto"
        );

    state.swing =
        doc["swing"] | false;

    Logger::info(
        "AC_STATE",
        "Loaded AC state for %s",
        deviceId.c_str()
    );

    return state;
}

bool ACStateService::updateState(
    const String& deviceId,
    const ACState& state
) {

    return saveState(
        deviceId,
        state
    );
}

ACState ACStateService::getState(
    const String& deviceId
) {

    return loadState(
        deviceId
    );
}