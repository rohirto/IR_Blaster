#include "api/action_routes.h"

#include <ArduinoJson.h>

#include "services/action_service.h"
#include "services/command_service.h"

#include "models/ir_command.h"
#include "config/system_config.h"
#include "config/api_paths.h"

#include "utils/logger.h"
#include "utils/json_utils.h"
#include "utils/validation_utils.h"

void registerActionRoutes(
    AsyncWebServer& server
) {

    // =====================================
    // POST /api/actions/execute
    // =====================================

    server.on(
        API_ACTION_EXECUTE,
        HTTP_POST,
        [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total)
        {
            JsonDocument doc;

            if (len > JsonUtils::MAX_JSON_DOC_SIZE)
            {
                    request->send(
                    413,
                    "application/json",
                    "{\"success\":false,\"error\":\"Payload too large\"}");

                return;
            }

            if (!JsonUtils::deserializeWithLengthValidation(
                    doc,
                    data,
                    len,
                    TAG_ACTION
                ))
            {

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

            if (!ValidationUtils::isValidId(deviceId))
            {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Invalid device id\"}");

                return;
            }

            if (!ValidationUtils::isValidAction(action))
            {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Invalid action\"}");

                return;
            }

            bool result =
                ActionService::executeAction(
                    deviceId,
                    action
                );

            if (!result)
            {

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
        });
    // =====================================
    // GET /api/actions/list
    // =====================================

    server.on(
        API_ACTION_LIST,
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