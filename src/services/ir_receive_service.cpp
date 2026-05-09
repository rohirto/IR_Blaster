#include "services/ir_receive_service.h"

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#include "utils/logger.h"
#include "config/pins.h"

#include "system/system_state.h"


// =====================================
// IR Receiver Instance
// =====================================

IRrecv irrecv(IR_RECEIVE_PIN);

decode_results results;

// =====================================
// Static Variables
// =====================================

bool IRReceiveService::newCommandAvailable =
    false;

IRCommand IRReceiveService::lastCommand;

// =====================================
// Begin
// =====================================

void IRReceiveService::begin() {

    irrecv.enableIRIn();

    Logger::info(
        TAG_IR_RECV,
        "IR receiver initialized on pin 12"
    );
}

// =====================================
// Loop
// =====================================

void IRReceiveService::loop() {
    if (
        SystemState::isOtaInProgress())
    {

        Logger::warn(
            "FS",
            "Write blocked during OTA");

        return;
    }

    if (!irrecv.decode(&results)) {

        return;
    }
    // Ignore repeat
    if (results.repeat) {
      irrecv.resume();
      return;
    }
    // Ignore noise
    if (results.bits < MIN_UNKNOWN_SIZE) {
      irrecv.resume();
      return;
    }
    // Ignore unknown protocols
    if (results.decode_type == UNKNOWN) {

        Logger::warn(
            TAG_IR_RECV,
            "Unknown IR protocol"
        );

        irrecv.resume();

        return;
    }

    String protocol =
        typeToString(results.decode_type);

    String data =
        uint64ToString(results.value, HEX);

    data.toUpperCase();

    uint16_t bits =
        results.bits;

    lastCommand = IRCommand(
        "",
        protocol,
        data,
        bits
    );

    newCommandAvailable = true;

    Logger::info(
        TAG_IR_RECV,
        "IR received | Protocol=%s Data=%s Bits=%d",
        protocol.c_str(),
        data.c_str(),
        bits
    );

    irrecv.resume();
}

// =====================================
// Has New Command
// =====================================

bool IRReceiveService::hasNewCommand() {

    return newCommandAvailable;
}

// =====================================
// Get Last Command
// =====================================

IRCommand IRReceiveService::getLastCommand() {

    return lastCommand;
}

// =====================================
// Clear Last Command
// =====================================

void IRReceiveService::clearLastCommand() {

    newCommandAvailable = false;
}