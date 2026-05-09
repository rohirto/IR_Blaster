#include "services/storage_service.h"

#include <LittleFS.h>

#include "utils/logger.h"

#include "system/system_state.h"

bool StorageService::begin() {

    if (!LittleFS.begin()) {

        Logger::error(
            TAG_FS,
            "LittleFS mount failed"
        );

        return false;
    }

    Logger::info(
        TAG_FS,
        "LittleFS mounted"
    );

    return true;
}

bool StorageService::exists(
    const String& path
) {

    return LittleFS.exists(path);
}

String StorageService::readFile(
    const String& path
) {

    File file = LittleFS.open(path, "r");

    if (!file) {

        Logger::warn(
            TAG_FS,
            "File not found: %s",
            path.c_str()
        );

        return "";
    }

    String content = file.readString();

    file.close();

    return content;
}

bool StorageService::writeFile(
    const String& path,
    const String& data
) {
    if (
        SystemState::isOtaInProgress())
    {

        Logger::warn(
            "FS",
            "Write blocked during OTA");

        return false;
    }

    String tempPath =
        path + ".tmp";

    File file =
        LittleFS.open(
            tempPath,
            "w"
        );

    if (!file) {

        Logger::error(
            "FS",
            "Failed to open temp file"
        );

        return false;
    }

    size_t written =
        file.print(data);

    file.close();

    if (written != data.length()) {

        Logger::error(
            "FS",
            "Incomplete write"
        );

        LittleFS.remove(tempPath);

        return false;
    }

    LittleFS.remove(path);

    if (!LittleFS.rename(tempPath, path)) {

        Logger::error(
            "FS",
            "Rename failed"
        );

        return false;
    }

    return true;
}
bool StorageService::deleteFile(
    const String& path
) {

    return LittleFS.remove(path);
}