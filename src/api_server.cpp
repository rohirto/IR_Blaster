#include "api_server.h"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

#include "utils/logger.h"
#include "config/system_config.h"

#include "api/system_routes.h"
#include "api/ir_routes.h"
#include "api/auth_routes.h"
#include "api/device_routes.h"
#include "api/config_routes.h"
#include "api/action_routes.h"
#include "api/ac_routes.h"
#include "api/scheduler_routes.h"

#include "system/system_state.h"

#include "services/alexa_service.h"

AsyncWebServer server(API_PORT);

void ApiServer::begin() {

    DefaultHeaders::Instance().addHeader(
        "Access-Control-Allow-Origin",
        "*"
    );

    // =============================
    // Existing Routes
    // =============================
    // Register modular routes
    registerSystemRoutes(server);

    registerIrRoutes(server);

    //registerAuthRoutes(server);

    registerDeviceRoutes(server);

    registerConfigRoutes(server);

    registerActionRoutes(server);

    registerAcRoutes(server);

    registerSchedulerRoutes(server);

    // 404 handler
    server.onNotFound([](AsyncWebServerRequest *request) {

        request->send(
            404,
            "application/json",
            "{\"success\":false,\"error\":\"Not Found\"}"
        );
    });

     // =============================
    // OTA Callbacks
    // =============================

    ElegantOTA.onStart([]() {

        Logger::info(
            "OTA",
            "OTA started"
        );

        SystemState::setOtaInProgress(
            true
        );
    });

    ElegantOTA.onEnd([](bool success) {

        Logger::info(
            "OTA",
            success
            ?
            "OTA successful"
            :
            "OTA failed"
        );

        SystemState::setOtaInProgress(
            false
        );
    });

    
    
    // =============================
    // Start Server
    // =============================
    AlexaService::begin(server);
    server.begin();

    // =============================
    // Initialize OTA
    // =============================
    ElegantOTA.begin(&server);

    Logger::info(
        TAG_API,
        "API server started"
    );
}

