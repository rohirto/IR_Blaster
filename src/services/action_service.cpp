#include "services/action_service.h"

#include "logger.h"

#include "models/ir_command.h"

#include "services/command_service.h"
#include "services/ir_send_service.h"

bool ActionService::executeAction(
    const String& deviceId,
    const String& action
) {

    Logger::info(
        "ACTION",
        "Executing action '%s' for device '%s'",
        action.c_str(),
        deviceId.c_str()
    );

    // =================================
    // Check Command Exists
    // =================================

    if (
        !CommandService::hasCommand(
            deviceId,
            action
        )
    ) {

        Logger::warn(
            "ACTION",
            "Command not found"
        );

        return false;
    }

    // =================================
    // Load Command
    // =================================

    IRCommand command =
        CommandService::getCommand(
            deviceId,
            action
        );

    // =================================
    // Send IR
    // =================================

    bool result =
        IRSendService::sendCommand(
            command
        );

    if (!result) {

        Logger::error(
            "ACTION",
            "IR transmission failed"
        );

        return false;
    }

    Logger::info(
        "ACTION",
        "Action executed successfully"
    );

    return true;
}