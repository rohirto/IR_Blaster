#pragma once

#include <Arduino.h>

class IRCommand {
public:

    String action;

    String protocol;

    String data;

    uint16_t bits;

    bool isValid;

    IRCommand()
        : bits(0),
          isValid(false) {
    }

    IRCommand(
        const String& action,
        const String& protocol,
        const String& data,
        uint16_t bits
    ) : action(action),
        protocol(protocol),
        data(data),
        bits(bits),
        isValid(true) {
    }
};