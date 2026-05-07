#pragma once

#include "models/ir_command.h"

class IRSendService {
public:

    static void begin();

    static bool sendCommand(
        const IRCommand& command
    );
};