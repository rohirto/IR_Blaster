#pragma once

#include <Arduino.h>

#include "models/ac_state.h"

class ACStateService {
public:

    // =================================
    // Load state from filesystem
    // =================================

    static ACState loadState(
        const String& deviceId
    );

    // =================================
    // Save state to filesystem
    // =================================

    static bool saveState(
        const String& deviceId,
        const ACState& state
    );

    // =================================
    // Update runtime state
    // =================================

    static bool updateState(
        const String& deviceId,
        const ACState& state
    );

    // =================================
    // Get runtime state
    // =================================

    static ACState getState(
        const String& deviceId
    );

private:

    static String getStateFilePath(
        const String& deviceId
    );
};