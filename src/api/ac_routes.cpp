#include "api/ac_routes.h"

#include <ArduinoJson.h>

#include "utils/logger.h"

#include "models/device.h"

#include "models/ac_state.h"

#include "services/device_service.h"

#include "services/ac/ac_state_service.h"

#include "services/ac/ac_controller_service.h"

#include "config/api_paths.h" 


void registerAcRoutes(
    AsyncWebServer& server
) {

    // =====================================
    // GET /api/ac/state
    // =====================================

    server.on(
        API_PATH_AC_STATE,
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

            ACState state =
                ACStateService::getState(
                    deviceId
                );

            JsonDocument doc;

            doc["success"] = true;

            doc["device_id"] =
                deviceId;

            doc["power"] =
                state.power;

            doc["temperature"] =
                state.temperature;

            doc["mode"] =
                state.modeToString();

            doc["fan"] =
                state.fanToString();

            doc["swing"] =
                state.swing;

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
    // POST /api/ac/set
    // =====================================

    server.on(
        API_PATH_AC_SET,
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

            if (deviceId.isEmpty()) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Missing device_id\"}"
                );

                return;
            }

            // =============================
            // Find Device
            // =============================

            Device* device =
                DeviceService::getDeviceById(
                    deviceId
                );

            if (device == nullptr) {

                request->send(
                    404,
                    "application/json",
                    "{\"success\":false,\"error\":\"Device not found\"}"
                );

                return;
            }

            // =============================
            // Build State
            // =============================

            ACState state;

            state.power =
                doc["power"] | false;

            state.temperature =
                doc["temperature"] | 24;

            state.mode =
                ACState::stringToMode(
                    doc["mode"] | "cool"
                );

            state.fan =
                ACState::stringToFan(
                    doc["fan"] | "auto"
                );

            state.swing =
                doc["swing"] | false;

            // =============================
            // Persist State
            // =============================

            ACStateService::updateState(
                deviceId,
                state
            );

            // =============================
            // Send IR Packet
            // =============================

            bool result =
                ACControllerService::sendState(
                    *device,
                    state
                );

            if (!result) {

                request->send(
                    500,
                    "application/json",
                    "{\"success\":false,\"error\":\"AC transmission failed\"}"
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


