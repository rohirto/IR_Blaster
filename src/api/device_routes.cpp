#include "api/device_routes.h"

#include <ArduinoJson.h>

#include "logger.h"

#include "models/device.h"

#include "services/device_service.h"

void registerDeviceRoutes(
    AsyncWebServer& server
) {

    // =====================================
    // GET /api/devices
    // =====================================

    server.on(
        "/api/devices",
        HTTP_GET,
        [](AsyncWebServerRequest *request) {

            std::vector<Device> devices =
                DeviceService::getDevices();

            JsonDocument doc;

            doc["success"] = true;

            JsonArray array =
                doc["devices"].to<JsonArray>();

            for (Device& device : devices) {

                JsonObject obj =
                    array.add<JsonObject>();

                obj["id"] = device.id;

                obj["name"] = device.name;

                obj["type"] = device.type;
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
    // POST /api/devices
    // =====================================

    server.on(
        "/api/devices",
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

            String id =
                doc["id"] | "";

            String name =
                doc["name"] | "";

            int type =
                doc["type"] | -1;

            if (
                id.isEmpty() ||
                name.isEmpty() ||
                type < 0
            ) {

                request->send(
                    400,
                    "application/json",
                    "{\"success\":false,\"error\":\"Missing fields\"}"
                );

                return;
            }

            Device device(
                id,
                name,
                (DeviceType)type
            );

            bool result =
                DeviceService::addDevice(
                    device
                );

            if (!result) {

                request->send(
                    500,
                    "application/json",
                    "{\"success\":false}"
                );

                return;
            }

            Logger::info(
                "DEVICE",
                "Device added: %s",
                id.c_str()
            );

            request->send(
                201,
                "application/json",
                "{\"success\":true}"
            );
        }
    );

    // =====================================
    // DELETE /api/devices?id=<id>
    // =====================================

    server.on(
        "/api/devices",
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
                DeviceService::deleteDevice(
                    id
                );

            if (!result) {

                request->send(
                    404,
                    "application/json",
                    "{\"success\":false,\"error\":\"Device not found\"}"
                );

                return;
            }

            Logger::info(
                "DEVICE",
                "Device deleted: %s",
                id.c_str()
            );

            request->send(
                200,
                "application/json",
                "{\"success\":true}"
            );
        }
    );
}