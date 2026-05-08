#include "services/ac/hitachi_ac_service.h"

#include <IRremoteESP8266.h>

#include <IRsend.h>

#include <ir_Hitachi.h>

#include "utils/logger.h"

#include "config/pins.h"

IRHitachiAc264 ac(
    IR_SEND_PIN
);

void HitachiAcService::begin() {

    ac.begin();

    Logger::info(
        "HITACHI_AC",
        "Hitachi AC service initialized"
    );
}

static uint8_t mapMode(
    ACMode mode
) {

    switch (mode) {

        case AC_MODE_COOL:
            return kHitachiAc264Cool;

        case AC_MODE_HEAT:
            return kHitachiAc264Heat;

        case AC_MODE_DRY:
            return kHitachiAc264Dry;

        case AC_MODE_FAN:
            return kHitachiAc264Fan;

        default:
            return kHitachiAc264Cool;
    }
}

static uint8_t mapFan(
    ACFanSpeed fan
) {

    switch (fan) {

        case AC_FAN_LOW:
            return kHitachiAc264FanMin;

        case AC_FAN_MEDIUM:
            return kHitachiAc264FanMedium;

        case AC_FAN_HIGH:
            return kHitachiAc264FanHigh;

        default:
            return kHitachiAc264FanAuto;
    }
}

bool HitachiAcService::sendState(
    const ACState& state
) {

    Logger::info(
        "HITACHI_AC",
        "Sending AC state | Power=%d Temp=%d Mode=%s Fan=%s Swing=%d",
        state.power,
        state.temperature,
        state.modeToString().c_str(),
        state.fanToString().c_str(),
        state.swing
    );

    // =================================
    // Power
    // =================================

    ac.setPower(
        state.power
    );

    // =================================
    // Temperature
    // =================================

    ac.setTemp(
        state.temperature
    );

    // =================================
    // Mode
    // =================================

    ac.setMode(
        mapMode(state.mode)
    );

    // =================================
    // Fan
    // =================================

    ac.setFan(
        mapFan(state.fan)
    );

    // =================================
    // Swing
    // =================================
    ac.setSwingVToggle(
        state.swing
    );

    // =================================
    // Send Packet
    // =================================

    ac.send();

    Logger::info(
        "HITACHI_AC",
        "AC packet transmitted"
    );

    return true;
}