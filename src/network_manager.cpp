#include "network_manager.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>

#include "utils/logger.h"
#include "config/system_config.h"
#include "config/constants.h"

String NetworkManager::hostname = "";

void NetworkManager::begin() {

    hostname = generateHostname();

    Logger::info(
        TAG_NET,
        "Generated hostname: %s",
        hostname.c_str()
    );

    WiFi.mode(WIFI_STA);

    WiFiManager wm;

    wm.setHostname(hostname.c_str());

    Logger::info(
        TAG_NET,
        "Starting WiFiManager..."
    );

    wm.setConnectTimeout(
        WIFI_CONNECT_TIMEOUT
        );


    bool result = wm.autoConnect(hostname.c_str());

    if (!result) {

        Logger::error(
            TAG_NET,
            "WiFi connection failed"
        );

        ESP.restart();
    }

    Logger::info(
        TAG_NET,
        "Connected to WiFi: %s",
        WiFi.SSID().c_str()
    );

    Logger::info(
        TAG_NET,
        "IP Address: %s",
        WiFi.localIP().toString().c_str()
    );

    setupMDNS();
}

void NetworkManager::loop() {

    MDNS.update();
}

String NetworkManager::getHostname() {
    return hostname;
}

void NetworkManager::setupMDNS() {

    if (!MDNS.begin(hostname.c_str())) {

        Logger::error(
            TAG_MDNS          ,
            "Failed to start mDNS"
        );

        return;
    }

    MDNS.addService("http", "tcp", API_PORT);

    Logger::info(
        TAG_MDNS,
        "mDNS started: http://%s.local",
        hostname.c_str()
    );
}

String NetworkManager::generateHostname() {

    uint8_t mac[6];

    WiFi.macAddress(mac);

    char hostnameBuffer[32];

    snprintf(
        hostnameBuffer,
        sizeof(hostnameBuffer),
        MDNS_PREFIX "%02X%02X%02X",
        mac[3],
        mac[4],
        mac[5]
    );

    return String(hostnameBuffer);
}

void NetworkManager::resetWiFi() {

    Logger::warn(
        TAG_NET,
        "Resetting WiFi settings"
    );

    WiFiManager wm;

    wm.resetSettings();

    delay(1000);

    ESP.restart();
}