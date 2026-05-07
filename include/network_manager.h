#pragma once

#include <Arduino.h>

class NetworkManager {
public:

    static void begin();

    static void loop();

    static String getHostname();

    static void resetWiFi();

private:

    static String hostname;

    static void setupMDNS();

    static String generateHostname();
};