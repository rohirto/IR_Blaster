#include "api/system_routes.h"

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "logger.h"
#include "network_manager.h"

void registerSystemRoutes(AsyncWebServer& server) {

    // =========================
    // Root
    // =========================

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

        request->send(
            200,
            "application/json",
            "{\"device\":\"IR Blaster\",\"status\":\"online\"}"
        );
    });

    // =========================
    // Ping
    // =========================

    server.on("/api/ping", HTTP_GET, [](AsyncWebServerRequest *request) {

        request->send(
            200,
            "application/json",
            "{\"success\":true,\"message\":\"pong\"}"
        );
    });

    // =========================
    // System Info
    // =========================

    server.on("/api/system", HTTP_GET, [](AsyncWebServerRequest *request) {

        DynamicJsonDocument doc(512);

        doc["success"] = true;

        JsonObject data = doc.createNestedObject("data");

        data["hostname"] = NetworkManager::getHostname();

        data["ip"] = WiFi.localIP().toString();

        data["ssid"] = WiFi.SSID();

        data["rssi"] = WiFi.RSSI();

        data["heap"] = ESP.getFreeHeap();

        data["uptime_ms"] = millis();

        String response;

        serializeJson(doc, response);

        request->send(
            200,
            "application/json",
            response
        );
    });

    // =========================
    // Restart
    // =========================

    server.on("/api/restart", HTTP_POST, [](AsyncWebServerRequest *request) {

        Logger::warn(
            "SYSTEM",
            "Restart requested"
        );

        request->send(
            200,
            "application/json",
            "{\"success\":true}"
        );

        delay(5000);

        ESP.restart();
    });
}