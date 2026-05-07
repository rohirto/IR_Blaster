#pragma once

#include <Arduino.h>

class StorageService {
public:

    static bool begin();

    static bool exists(
        const String& path
    );

    static String readFile(
        const String& path
    );

    static bool writeFile(
        const String& path,
        const String& content
    );

    static bool deleteFile(
        const String& path
    );
};