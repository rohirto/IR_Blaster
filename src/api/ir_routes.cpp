#include "api/ir_routes.h"

#include <ArduinoJson.h>

#include "models/ir_command.h"

#include "services/command_service.h"

void registerIrRoutes(
    AsyncWebServer& server
) {

    // =====================================
    // GET /api/commands
    // =====================================

    server.on(
        "/api/commands",
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

            doc["device_id"] = deviceId;

            JsonArray array =
                doc["commands"].to<JsonArray>();

            for (IRCommand& cmd : commands) {

                JsonObject obj =
                    array.add<JsonObject>();

                obj["action"] =
                    cmd.action;

                obj["protocol"] =
                    cmd.protocol;

                obj["data"] =
                    cmd.data;

                obj["bits"] =
                    cmd.bits;
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
        // =====================================
    // DELETE /api/commands
    // =====================================

    server.on(
        "/api/commands",
        HTTP_DELETE,
        [](AsyncWebServerRequest *request) {

            if (
                !request->hasParam("device_id") ||
                !request->hasParam("action")
            ) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Missing parameters\"}"
                );

                return;
            }

            String deviceId =
                request->getParam("device_id")->value();

            String action =
                request->getParam("action")->value();

            bool result =
                CommandService::deleteCommand(
                    deviceId,
                    action
                );

            if (!result) {

                request->send(
                    404,
                    "application/json",
                    "{\"success\":false,\"error\":\"Delete failed\"}"
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