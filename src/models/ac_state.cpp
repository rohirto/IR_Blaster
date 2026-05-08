#include "models/ac_state.h"

// =====================================
// Mode To String
// =====================================

String ACState::modeToString() const {

    switch (mode) {

        case AC_MODE_COOL:
            return "cool";

        case AC_MODE_HEAT:
            return "heat";

        case AC_MODE_DRY:
            return "dry";

        case AC_MODE_FAN:
            return "fan";

        case AC_MODE_AUTO:
            return "auto";

        default:
            return "unknown";
    }
}

// =====================================
// Fan To String
// =====================================

String ACState::fanToString() const {

    switch (fan) {

        case AC_FAN_AUTO:
            return "auto";

        case AC_FAN_LOW:
            return "low";

        case AC_FAN_MEDIUM:
            return "medium";

        case AC_FAN_HIGH:
            return "high";

        default:
            return "unknown";
    }
}

ACMode ACState::stringToMode(
    const String& mode
) {

    if (mode == "cool")
        return AC_MODE_COOL;

    if (mode == "heat")
        return AC_MODE_HEAT;

    if (mode == "dry")
        return AC_MODE_DRY;

    if (mode == "fan")
        return AC_MODE_FAN;

    return AC_MODE_AUTO;
}

ACFanSpeed ACState::stringToFan(
    const String& fan
) {

    if (fan == "low")
        return AC_FAN_LOW;

    if (fan == "medium")
        return AC_FAN_MEDIUM;

    if (fan == "high")
        return AC_FAN_HIGH;

    return AC_FAN_AUTO;
}