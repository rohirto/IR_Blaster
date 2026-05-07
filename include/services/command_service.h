#pragma once

#include <vector>

#include "models/ir_command.h"

class CommandService {
public:

    static bool saveCommand(
        const String& deviceId,
        const IRCommand& command
    );

    static bool hasCommand(
        const String& deviceId,
        const String& action
    );

    static IRCommand getCommand(
        const String& deviceId,
        const String& action
    );

    static std::vector<IRCommand> getCommands(
        const String& deviceId
    );

    static bool deleteCommand(
        const String& deviceId,
        const String& action
    );

    static bool deleteCommands(
        const String& deviceId
    );

private:

    static String getCommandFilePath(
        const String& deviceId
    );
};