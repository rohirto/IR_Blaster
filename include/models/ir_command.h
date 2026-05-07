#pragma once

#include <Arduino.h>

class IRCommand {
public:

    String action;

    String protocol;

    String data;

    uint16_t bits;

    IRCommand();

    IRCommand(
        String action,
        String protocol,
        String data,
        uint16_t bits
    );
};