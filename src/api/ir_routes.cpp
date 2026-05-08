#include "api/ir_routes.h"

#include <ArduinoJson.h>

#include "models/ir_command.h"

#include "services/command_service.h"
#include "config/api_paths.h"

#include "utils/logger.h"
#include "config/system_config.h"
#include "utils/ir_utils.h"

void registerIrRoutes(
    AsyncWebServer& server
) {

    // =====================================
    // GET /api/commands
    // =====================================

    server.on(
        API_COMMANDS,
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
        API_COMMANDS,
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
    // =====================================
    // POST /api/commands
    // =====================================
    server.on(
        API_COMMANDS,
        HTTP_POST,
        [](AsyncWebServerRequest *request) {
        },
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {
            // =============================
            // Payload Size Validation
            // =============================
            if (len > MAX_JSON_DOC_SIZE)
            {

                Logger::warn(
                    TAG_COMMAND,
                    "Payload too large"
                );
                request->send(
                    413,
                    "application/json",
                    "{\"success\":false,\"error\":\"Payload too large\"}"
                );
                return;
            }
            JsonDocument doc;

            DeserializationError error =
                deserializeJson(
                    doc,
                    data
                );
            if (error)
            {

                Logger::error(
                    TAG_COMMAND,
                    "JSON parse failed: %s",
                    error.c_str()
                );
                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Invalid JSON\"}"
                );

                return;
            }
            // =============================
            // Required Fields
            // =============================
            String deviceId =
                doc["device_id"] | "";

            String action =
                doc["action"] | "";

            String protocol =
                doc["protocol"] | "";

            String irData =
                doc["data"] | "";

            uint16_t bits =
                doc["bits"] | 0;
            if (
                deviceId.isEmpty() ||
                action.isEmpty() ||
                protocol.isEmpty() ||
                irData.isEmpty() ||
                bits == 0
            ) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Missing fields\"}"
                );

                return;
            }
            // =============================
            // Protocol Validation
            // =============================

            if (
                !IRUtils::isSupportedProtocol(
                    protocol
                )
            ) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Unsupported protocol\"}"
                );

                return;
            }
            // =============================
            // Build Command
            // =============================

            IRCommand command(
                action,
                protocol,
                irData,
                bits
            );
            bool result =
                CommandService::saveCommand(
                    deviceId,
                    command
                );

            if (!result)
            {

                request->send(
                    500,
                    "application/json",
                    "{\"success\":false,\"error\":\"Failed to save command\"}"
                );

                return;
            }
            Logger::info(
                TAG_COMMAND,
                "Manual IR command imported"
            );

            request->send(
                200,
                "application/json",
                "{\"success\":true}"
            );
        }
    );


}