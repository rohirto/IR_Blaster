#pragma once

#include <Arduino.h>

class ActionService {
public:

    static bool executeAction(
        const String& deviceId,
        const String& action
    );
};