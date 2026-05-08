#include "api_server.h"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "logger.h"
#include "config/system_config.h"

#include "api/system_routes.h"
#include "api/ir_routes.h"
#include "api/auth_routes.h"
#include "api/device_routes.h"
#include "api/config_routes.h"
#include "api/action_routes.h"

AsyncWebServer server(API_PORT);

void ApiServer::begin() {

    DefaultHeaders::Instance().addHeader(
        "Access-Control-Allow-Origin",
        "*"
    );

    // Register modular routes
    registerSystemRoutes(server);

    registerIrRoutes(server);

    //registerAuthRoutes(server);

    registerDeviceRoutes(server);

    registerConfigRoutes(server);

    registerActionRoutes(server);

    // 404 handler
    server.onNotFound([](AsyncWebServerRequest *request) {

        request->send(
            404,
            "application/json",
            "{\"success\":false,\"error\":\"Not Found\"}"
        );
    });

    server.begin();

    Logger::info(
        "API",
        "API server started"
    );
}