#pragma once

#include <Arduino.h>

// =====================================
// AC Modes
// =====================================

enum ACMode {

    AC_MODE_COOL = 0,

    AC_MODE_HEAT = 1,

    AC_MODE_DRY = 2,

    AC_MODE_FAN = 3,

    AC_MODE_AUTO = 4
};

// =====================================
// Fan Speeds
// =====================================

enum ACFanSpeed {

    AC_FAN_AUTO = 0,

    AC_FAN_LOW = 1,

    AC_FAN_MEDIUM = 2,

    AC_FAN_HIGH = 3
};

// =====================================
// AC State Model
// =====================================

class ACState {
public:

    bool power = false;

    uint8_t temperature = 24;

    ACMode mode = AC_MODE_COOL;

    ACFanSpeed fan = AC_FAN_AUTO;

    bool swing = false;

    // =================================
    // Helpers
    // =================================

    String modeToString() const;

    String fanToString() const;

    static ACMode stringToMode(
        const String& mode
    );

    static ACFanSpeed stringToFan(
        const String& fan
    );
};