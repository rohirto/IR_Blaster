#pragma once

#include <Arduino.h>

#include "models/ir_command.h"

class IRReceiveService {
public:

    static void begin();

    static void loop();

    static bool hasNewCommand();

    static IRCommand getLastCommand();

    static void clearLastCommand();

private:

    static bool newCommandAvailable;

    static IRCommand lastCommand;
};