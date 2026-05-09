#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "models/ac_state.h"
#include "utils/json_utils.h"

// =====================================
// Task Types
// =====================================

enum ScheduledTaskType {

    TASK_TYPE_AC_STATE = 0
};

// =====================================
// Scheduled Task
// =====================================

class ScheduledTask {
public:

    // =================================
    // Identity
    // =================================

    String id;

    // =================================
    // Execution
    // =================================

    uint32_t executeAt = 0;

    bool executed = false;

    uint32_t lastAttemptAt = 0;

    uint8_t retryCount = 0;

    bool failed = false;

    // =================================
    // Device
    // =================================

    String deviceId;

    // =================================
    // Task Type
    // =================================

    ScheduledTaskType type =
        TASK_TYPE_AC_STATE;

    // =================================
    // AC State Payload
    // =================================

    ACState acState;

    JsonObject toJson(
        JsonObject obj
    ) const;

    static ScheduledTask fromJson(
        JsonObject obj
    );
};