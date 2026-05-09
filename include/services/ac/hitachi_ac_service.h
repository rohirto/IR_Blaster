#pragma once

#include "models/ac_state.h"

class HitachiAcService {
public:

    static void begin();

    static bool sendState(
        ACState state
    );
};