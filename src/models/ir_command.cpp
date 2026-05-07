#include "models/ir_command.h"

IRCommand::IRCommand() {
}

IRCommand::IRCommand(
    String action,
    String protocol,
    String data,
    uint16_t bits
) {

    this->action = action;

    this->protocol = protocol;

    this->data = data;

    this->bits = bits;
}