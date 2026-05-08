#include "services/storage_service.h"

#include <LittleFS.h>

#include "utils/logger.h"

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
    const String& content
) {

    File file = LittleFS.open(path, "w");

    if (!file) {

        Logger::error(
            TAG_FS,
            "Failed writing file: %s",
            path.c_str()
        );

        return false;
    }

    file.print(content);

    file.close();

    return true;
}

bool StorageService::deleteFile(
    const String& path
) {

    return LittleFS.remove(path);
}