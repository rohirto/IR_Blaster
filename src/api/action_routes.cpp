#include "api/action_routes.h"

#include <ArduinoJson.h>

#include "services/action_service.h"
#include "services/command_service.h"

#include "models/ir_command.h"

void registerActionRoutes(
    AsyncWebServer& server
) {

    // =====================================
    // POST /api/actions/execute
    // =====================================

    server.on(
        "/api/actions/execute",
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

            String deviceId =
                doc["device_id"] | "";

            String action =
                doc["action"] | "";

            if (
                deviceId.isEmpty() ||
                action.isEmpty()
            ) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Missing fields\"}"
                );

                return;
            }

            bool result =
                ActionService::executeAction(
                    deviceId,
                    action
                );

            if (!result) {

                request->send(
                    404,
                    "application/json",
                    "{\"success\":false,\"error\":\"Action execution failed\"}"
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
    // GET /api/actions/list
    // =====================================

    server.on(
        "/api/actions/list",
        HTTP_GET,
        [](AsyncWebServerRequest *request) {

            if (!request->hasParam("device_id")) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Missing device_id\"}"
                );

                return;
            }

            String deviceId =
                request->getParam("device_id")->value();

            std::vector<IRCommand> commands =
                CommandService::getCommands(
                    deviceId
                );

            JsonDocument doc;

            doc["success"] = true;

            doc["device_id"] =
                deviceId;

            JsonArray actions =
                doc["actions"]
                    .to<JsonArray>();

            for (IRCommand& cmd : commands) {

                JsonObject obj =
                    actions.add<JsonObject>();

                obj["action"] =
                    cmd.action;

                // =================================
                // Human Readable Label
                // =================================

                String displayName =
                    cmd.action;

                displayName.replace("_", " ");

                if (displayName.length() > 0) {

                    displayName[0] =
                        toupper(displayName[0]);
                }

                obj["display_name"] =
                    displayName;
            }

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
}