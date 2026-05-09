#pragma once

#include <vector>

#include "models/scheduled_task.h"

class SchedulerService {
public:

    // =================================
    // Initialization
    // =================================

    static void begin();

    // =================================
    // Main Scheduler Tick
    // =================================

    static void loop();

    // =================================
    // Task Management
    // =================================

    static bool addTask(
        const ScheduledTask& task
    );

    static bool removeTask(
        const String& id
    );

    static std::vector<ScheduledTask>
    getTasks();

    // =================================
    // Due Task Retrieval
    // =================================

    static std::vector<ScheduledTask>
    getDueTasks();

private:

    static std::vector<ScheduledTask>
        tasks;
    static void executeTask(
        ScheduledTask& task
    );

    static bool saveTasks();

    static bool loadTasks();

    static void cleanupExecutedTasks();
};