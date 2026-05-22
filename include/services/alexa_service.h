#pragma once
#include <ESP8266WiFi.h>
#define ESPALEXA_ASYNC
#include <Espalexa.h>
#include <ESPAsyncWebServer.h>

extern Espalexa espalexa;
class AlexaService {
public:

    static void begin(AsyncWebServer& server);

    static bool handleRequest(AsyncWebServerRequest* request);

    static void loop();
};