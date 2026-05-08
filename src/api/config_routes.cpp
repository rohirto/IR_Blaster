#include "api/config_routes.h"

#include <ArduinoJson.h>

#include "services/mode_service.h"
#include "services/learn_service.h"
#include "config/system_config.h"
#include "config/api_paths.h"

#include "utils/logger.h"
#include "utils/json_utils.h"
#include "utils/validation_utils.h"

void registerConfigRoutes(
    AsyncWebServer& server
) {

    // =====================================
    // GET /api/config/status
    // =====================================

    server.on(
        API_CONFIG_STATUS,
        HTTP_GET,
        [](AsyncWebServerRequest *request) {

            JsonDocument doc;

            doc["success"] = true;

            doc["mode"] =
                ModeService::getModeString();

            doc["config_mode"] =
                ModeService::isConfigMode();

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
    // POST /api/config/start
    // =====================================

    server.on(
        API_CONFIG_START,
        HTTP_POST,
        [](AsyncWebServerRequest *request) {

            ModeService::setMode(
                CONFIG_MODE
            );

            request->send(
                200,
                "application/json",
                "{\"success\":true}"
            );
        }
    );

    // =====================================
    // POST /api/config/end
    // =====================================

    server.on(
        API_CONFIG_END,
        HTTP_POST,
        [](AsyncWebServerRequest *request) {

            LearnService::cancelLearning();

            ModeService::setMode(
                NORMAL_MODE
            );

            request->send(
                200,
                "application/json",
                "{\"success\":true}"
            );
        }
    );

    // =====================================
    // POST /api/config/learn
    // =====================================

    server.on(
        API_CONFIG_LEARN,
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
                    TAG_LEARN
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
                LearnService::startLearning(
                    deviceId,
                    action
                );

            if (!result) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Failed to start learning\"}"
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
    // GET /api/config/learn/status
    // =====================================

    server.on(
        API_CONFIG_LEARN_STATUS,
        HTTP_GET,
        [](AsyncWebServerRequest *request) {

            JsonDocument doc;

            doc["success"] = true;

            doc["state"] =
                LearnService::getStateString();

            doc["device_id"] =
                LearnService::getCurrentDeviceId();

            doc["action"] =
                LearnService::getCurrentAction();

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
    // POST /api/config/learn/cancel
    // =====================================

    server.on(
        API_CONFIG_LEARN_CANCEL,
        HTTP_POST,
        [](AsyncWebServerRequest *request) {

            LearnService::cancelLearning();

            request->send(
                200,
                "application/json",
                "{\"success\":true}"
            );
        }
    );
}