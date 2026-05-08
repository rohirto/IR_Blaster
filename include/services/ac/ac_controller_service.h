#pragma once

#include <Arduino.h>

#include "models/device.h"

#include "models/ac_state.h"

class ACControllerService {
public:

    static bool sendState(
        const Device& device,
        const ACState& state
    );
};