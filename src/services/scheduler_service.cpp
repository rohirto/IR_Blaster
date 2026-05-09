#include "services/scheduler_service.h"

#include <time.h>
#include <algorithm>

#include "utils/logger.h"
#include "services/device_service.h"

#include "services/ac/ac_state_service.h"

#include "services/ac/ac_controller_service.h"

#include <ArduinoJson.h>

#include "services/storage_service.h"

#include "utils/json_utils.h"

#include "config/constants.h"

std::vector<ScheduledTask>
SchedulerService::tasks;

void SchedulerService::begin() {

    loadTasks();
    Logger::info(
        "SCHED",
        "Scheduler initialized"
    );
}

bool SchedulerService::addTask(
    const ScheduledTask &task)
{
    for (const ScheduledTask &t : tasks)
    {

        if (t.id == task.id)
        {

            Logger::warn(
                "SCHED",
                "Duplicate task ID rejected");

            return false;
        }
    }

    tasks.push_back(task);

    saveTasks();

    Logger::info(
        "SCHED",
        "Task added: %s",
        task.id.c_str()
    );

    return true;
}

bool SchedulerService::removeTask(
    const String& id
) {

    for (
        auto it = tasks.begin();
        it != tasks.end();
        ++it
    ) {

        if (it->id == id) {

            tasks.erase(it);
            saveTasks();

            Logger::info(
                "SCHED",
                "Task removed: %s",
                id.c_str()
            );

            return true;
        }
    }

    Logger::warn(
        "SCHED",
        "Task not found: %s",
        id.c_str()
    );

    return false;
}

std::vector<ScheduledTask>
SchedulerService::getTasks() {

    return tasks;
}

std::vector<ScheduledTask>
SchedulerService::getDueTasks() {

    std::vector<ScheduledTask> due;

    uint32_t now =
        time(nullptr);

    for (
        ScheduledTask& task :
        tasks
    ) {

        if (
            !task.executed &&
            now >= task.executeAt
        ) {

            due.push_back(task);
        }
    }

    return due;
}

void SchedulerService::executeTask(
    ScheduledTask& task
) {
    task.lastAttemptAt =
        time(nullptr);

    task.retryCount++;

    Logger::info(
        "SCHED",
        "Executing task: %s",
        task.id.c_str()
    );

    // =================================
    // Find Device
    // =================================

    Device* device =
        DeviceService::getDeviceById(
            task.deviceId
        );

    if (device == nullptr)
    {

        Logger::error(
            "SCHED",
            "Device not found");

        if (
            task.retryCount >=
            SCHEDULER_MAX_RETRIES)
        {

            task.failed = true;
            return;

            Logger::error(
                "SCHED",
                "Task permanently failed");

            saveTasks();
        }

        return;
    }

    // =================================
    // Execute Task Type
    // =================================

    switch (task.type) {

        // =============================
        // AC State Task
        // =============================

        case TASK_TYPE_AC_STATE: {

            // -------------------------
            // Persist State
            // -------------------------

            ACStateService::updateState(
                task.deviceId,
                task.acState
            );

            // -------------------------
            // Send IR Packet
            // -------------------------

            bool result =
                ACControllerService::sendState(
                    *device,
                    task.acState
                );

            if (!result) {

                Logger::error(
                    "SCHED",
                    "AC transmission failed"
                );

                return;
            }

            Logger::info(
                "SCHED",
                "AC task executed successfully"
            );

            break;
        }

        // =============================
        // Unsupported
        // =============================

        default:

            Logger::warn(
                "SCHED",
                "Unsupported task type"
            );

            return;
    }

    // =================================
    // Mark Executed
    // =================================

    task.executed = true;
    saveTasks();
}

void SchedulerService::loop() {

    uint32_t now =
        time(nullptr);

    for (
        ScheduledTask &task :
        tasks)
    {

        // =============================
        // Terminal States
        // =============================

        if (
            task.executed ||
            task.failed)
        {

            continue;
        }

        // =============================
        // Not Yet Due
        // =============================

        if (
            now < task.executeAt)
        {

            continue;
        }

        // =============================
        // Retry Cooldownś
        // =============================
        uint32_t now = time(nullptr);

        if (
            task.lastAttemptAt != 0 &&
            now <
                task.lastAttemptAt +
                    SCHEDULER_RETRY_DELAY_SEC)
        {

            continue;
        }

        executeTask(task);
    }

    cleanupExecutedTasks();
}

bool SchedulerService::saveTasks() {

    JsonDocument doc;

    JsonArray array =
        doc["tasks"]
            .to<JsonArray>();

    for (
        const ScheduledTask& task :
        tasks
    ) {

        JsonObject obj =
            array.add<JsonObject>();

        task.toJson(obj);
    }

    String output;

    serializeJsonPretty(
        doc,
        output
    );

    Logger::debug(
        "SCHED",
        "Saving tasks"
    );

    return StorageService::writeFile(
        SCHEDULER_FILE,
        output
    );
}

bool SchedulerService::loadTasks() {

    if (
        !StorageService::exists(
            SCHEDULER_FILE
        )
    ) {

        Logger::warn(
            "SCHED",
            "Scheduler file not found"
        );

        return true;
    }

    String json =
        StorageService::readFile(
            SCHEDULER_FILE
        );

    JsonDocument doc;

    if (
        !JsonUtils::deserialize(
            doc,
            json,
            "SCHED"
        )
    ) {

        return false;
    }

    JsonArray array =
        doc["tasks"]
            .as<JsonArray>();

    tasks.clear();

    for (JsonObject obj : array) {

        tasks.push_back(
            ScheduledTask::fromJson(obj)
        );
    }

    Logger::info(
        "SCHED",
        "Loaded %d tasks",
        tasks.size()
    );

    return true;
}

void SchedulerService::cleanupExecutedTasks() {

    size_t before =
        tasks.size();

    tasks.erase(

        std::remove_if(

            tasks.begin(),
            tasks.end(),

            [](const ScheduledTask& task) {

                return task.executed || task.failed;
            }

        ),

        tasks.end()
    );

    size_t removed =
        before - tasks.size();

    if (removed > 0) {

        Logger::info(
            "SCHED",
            "Cleaned up %d executed tasks",
            removed
        );

        saveTasks();
    }
}