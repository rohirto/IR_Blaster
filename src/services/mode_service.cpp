#include "services/mode_service.h"

#include "utils/logger.h"

SystemMode ModeService::currentMode =
    NORMAL_MODE;

void ModeService::begin() {

    currentMode = NORMAL_MODE;

    Logger::info(
        TAG_MODE,
        "Initialized in NORMAL_MODE"
    );
}

void ModeService::setMode(
    SystemMode mode
) {

    currentMode = mode;

    Logger::info(
        TAG_MODE,
        "Mode changed to: %s",
        getModeString()
    );
}

SystemMode ModeService::getMode() {

    return currentMode;
}

bool ModeService::isNormalMode() {

    return currentMode == NORMAL_MODE;
}

bool ModeService::isConfigMode() {

    return currentMode == CONFIG_MODE;
}

const char* ModeService::getModeString() {

    switch (currentMode) {

        case NORMAL_MODE:
            return "NORMAL_MODE";

        case CONFIG_MODE:
            return "CONFIG_MODE";

        default:
            return "UNKNOWN";
    }
}