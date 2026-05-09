#include "system/system_state.h"

bool SystemState::otaInProgress =
    false;

bool SystemState::isOtaInProgress() {

    return otaInProgress;
}

void SystemState::setOtaInProgress(
    bool value
) {

    otaInProgress = value;
}