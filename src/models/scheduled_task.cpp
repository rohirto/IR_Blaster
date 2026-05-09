#include "models/scheduled_task.h"

JsonObject ScheduledTask::toJson(
    JsonObject obj
) const {

    obj["id"] =
        id;

    obj["execute_at"] =
        executeAt;

    obj["executed"] =
        executed;

    obj["device_id"] =
        deviceId;

    obj["type"] =
        type;

    // =============================
    // AC State
    // =============================

    JsonObject ac =
        obj["ac_state"]
            .to<JsonObject>();

    ac["power"] =
        acState.power;

    ac["temperature"] =
        acState.temperature;

    ac["mode"] =
        acState.modeToString();

    ac["fan"] =
        acState.fanToString();

    ac["swing"] =
        acState.swing;

    return obj;
}

ScheduledTask ScheduledTask::fromJson(
    JsonObject obj
) {

    ScheduledTask task;

    task.id =
        obj["id"] | "";

    task.executeAt =
        obj["execute_at"] | 0;

    task.executed =
        obj["executed"] | false;

    task.deviceId =
        obj["device_id"] | "";

    task.type =
        (ScheduledTaskType)(
            obj["type"] | 0
        );

    // =============================
    // AC State
    // =============================

    JsonObject ac =
        obj["ac_state"];

    task.acState.power =
        ac["power"] | false;

    task.acState.temperature =
        ac["temperature"] | 24;

    task.acState.mode =
        ACState::stringToMode(
            ac["mode"] | "cool"
        );

    task.acState.fan =
        ACState::stringToFan(
            ac["fan"] | "auto"
        );

    task.acState.swing =
        ac["swing"] | false;

    return task;
}


