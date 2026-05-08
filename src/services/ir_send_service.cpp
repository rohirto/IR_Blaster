#include "services/ir_send_service.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "logger.h"

// =====================================
// IR LED Pin
// =====================================

static const uint16_t IR_SEND_PIN = 14;

// =====================================
// IR Sender Instance
// =====================================

IRsend irsend(IR_SEND_PIN);

// =====================================
// Begin
// =====================================

void IRSendService::begin() {

    irsend.begin();

    Logger::info(
        "IR_SEND",
        "IR sender initialized on pin %d",
        IR_SEND_PIN
    );
}

// =====================================
// Send Command
// =====================================

bool IRSendService::sendCommand(
    const IRCommand& command
) {

    uint64_t value =
        strtoull(
            command.data.c_str(),
            NULL,
            16
        );

    Logger::info(
        "IR_SEND",
        "Sending IR | Protocol=%s Data=%s Bits=%d",
        command.protocol.c_str(),
        command.data.c_str(),
        command.bits
    );

    // =================================
    // NEC
    // =================================

    if (command.protocol == "NEC") {

        irsend.sendNEC(
            value,
            command.bits
        );

        return true;
    }

    // =================================
    // SONY
    // =================================

    if (command.protocol == "SONY") {

        irsend.sendSony(
            value,
            command.bits
        );

        return true;
    }

    // =================================
    // SAMSUNG
    // =================================

    if (command.protocol == "SAMSUNG") {
        irsend.sendSAMSUNG(
            value,
            command.bits
        );

        return true;
    }

    // =================================
    // Unsupported Protocol
    // =================================

    Logger::warn(
        "IR_SEND",
        "Unsupported protocol: %s",
        command.protocol.c_str()
    );

    return false;
}