#include "services/learn_service.h"

#include "utils/logger.h"
#include "config/system_config.h"

#include "services/mode_service.h"
#include "services/ir_receive_service.h"
#include "services/command_service.h"

LearnState LearnService::currentState =
    LEARN_IDLE;

String LearnService::currentDeviceId =
    "";

String LearnService::currentAction =
    "";

unsigned long LearnService::learnStartTime =
    0;


// =====================================
// Begin
// =====================================

void LearnService::begin() {

    Logger::info(
        TAG_LEARN,
        "LearnService initialized"
    );
}

// =====================================
// Loop
// =====================================

void LearnService::loop() {

    if (currentState != LEARN_WAITING) {

        return;
    }

    // =================================
    // Timeout
    // =================================

    if (
        millis() - learnStartTime >
        LEARN_TIMEOUT_MS
    ) {

        Logger::warn(
            TAG_LEARN,
            "Learning timeout"
        );

        currentState = LEARN_FAILED;

        return;
    }

    // =================================
    // New IR Command
    // =================================

    if (!IRReceiveService::hasNewCommand()) {

        return;
    }

    IRCommand cmd =
        IRReceiveService::getLastCommand();

    cmd.action =
        currentAction;

    bool result =
        CommandService::saveCommand(
            currentDeviceId,
            cmd
        );

    IRReceiveService::clearLastCommand();

    if (!result) {

        Logger::error(
            TAG_LEARN,
            "Failed to save learned command"
        );

        currentState = LEARN_FAILED;

        return;
    }

    Logger::info(
        TAG_LEARN,
        "Learned action '%s' for device '%s'",
        currentAction.c_str(),
        currentDeviceId.c_str()
    );

    currentState = LEARN_SUCCESS;
}

// =====================================
// Start Learning
// =====================================

bool LearnService::startLearning(
    const String& deviceId,
    const String& action
) {

    if (!ModeService::isConfigMode()) {

        Logger::warn(
            TAG_LEARN,
            "Cannot learn outside CONFIG_MODE"
        );

        return false;
    }

    currentDeviceId =
        deviceId;

    currentAction =
        action;

    currentState =
        LEARN_WAITING;

    learnStartTime =
        millis();

    IRReceiveService::clearLastCommand();

    Logger::info(
        TAG_LEARN,
        "Started learning | Device=%s Action=%s",
        deviceId.c_str(),
        action.c_str()
    );

    return true;
}

// =====================================
// Cancel Learning
// =====================================

void LearnService::cancelLearning() {

    currentState =
        LEARN_IDLE;

    currentDeviceId =
        "";

    currentAction =
        "";

    Logger::info(
        TAG_LEARN,
        "Learning cancelled"
    );
}

// =====================================
// Get State
// =====================================

LearnState LearnService::getState() {

    return currentState;
}

// =====================================
// State String
// =====================================

const char* LearnService::getStateString() {

    switch (currentState) {

        case LEARN_IDLE:
            return "IDLE";

        case LEARN_WAITING:
            return "WAITING";

        case LEARN_SUCCESS:
            return "SUCCESS";

        case LEARN_FAILED:
            return "FAILED";

        default:
            return "UNKNOWN";
    }
}

// =====================================
// Current Device
// =====================================

String LearnService::getCurrentDeviceId() {

    return currentDeviceId;
}

// =====================================
// Current Action
// =====================================

String LearnService::getCurrentAction() {

    return currentAction;
}