#include "api/scheduler_routes.h"

#include <ArduinoJson.h>

#include "utils/logger.h"

#include "models/scheduled_task.h"

#include "services/scheduler_service.h"

#include "models/ac_state.h"

void registerSchedulerRoutes(
    AsyncWebServer& server
) {

    // =====================================
    // GET /api/scheduler/tasks
    // =====================================

    server.on(
        "/api/scheduler/tasks",
        HTTP_GET,
        [](AsyncWebServerRequest *request) {

            std::vector<ScheduledTask>
                tasks =
                    SchedulerService::getTasks();

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

            doc["success"] = true;

            String response;

            serializeJsonPretty(
                doc,
                response
            );

            request->send(
                200,
                "application/json",
                response
            );
        }
    );

        // =====================================
    // POST /api/scheduler/tasks
    // =====================================

    server.on(
        "/api/scheduler/tasks",
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {

            JsonDocument doc;

            DeserializationError error =
                deserializeJson(
                    doc,
                    data
                );

            if (error) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Invalid JSON\"}"
                );

                return;
            }

            ScheduledTask task;

            // =============================
            // Identity
            // =============================

            task.id =
                doc["id"] | "";

            task.deviceId =
                doc["device_id"] | "";

            task.executeAt =
                doc["execute_at"] | 0;

            task.type =
                (ScheduledTaskType)(
                    doc["type"] | 0
                );

            // =============================
            // AC State
            // =============================

            JsonObject ac =
                doc["ac_state"];

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

            // =============================
            // Add Task
            // =============================

            bool result =
                SchedulerService::addTask(
                    task
                );

            if (!result) {

                request->send(
                    500,
                    "application/json",
                    "{\"success\":false}"
                );

                return;
            }

            request->send(
                200,
                "application/json",
                "{\"success\":true}"
            );
        }
    );

        // =====================================
    // DELETE /api/scheduler/tasks?id=...
    // =====================================

    server.on(
        "/api/scheduler/tasks",
        HTTP_DELETE,
        [](AsyncWebServerRequest *request) {

            if (!request->hasParam("id")) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Missing id\"}"
                );

                return;
            }

            String id =
                request->getParam("id")->value();

            bool result =
                SchedulerService::removeTask(
                    id
                );

            if (!result) {

                request->send(
                    404,
                    "application/json",
                    "{\"success\":false}"
                );

                return;
            }

            request->send(
                200,
                "application/json",
                "{\"success\":true}"
            );
        }
    );
}