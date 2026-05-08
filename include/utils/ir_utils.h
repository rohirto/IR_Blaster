#pragma once

#include <Arduino.h>

class IRUtils {
public:

    static bool isSupportedProtocol(
        const String& protocol
    );
};