#include "services/alexa_service.h"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <map>

#include "utils/logger.h"

#include "services/device_service.h"

#include "services/action_service.h"

#include "services/ac/ac_controller_service.h"

Espalexa espalexa;

static std::map<EspalexaDevice*,String> alexaDeviceMap;

// void alexaDeviceCallback(
//     uint8_t brightness
// ) {

//     String deviceName =
//         "Living Room TV";

//     Device* device =
//         DeviceService::getDeviceByName(
//             deviceName
//         );

//     if (device == nullptr) {

//         return;
//     }

//     if (brightness > 0) {

//         ActionService::executeAction(
//             device->id,
//             "power_on"
//         );

//     } else {

//         ActionService::executeAction(
//             device->id,
//             "power_off"
//         );
//     }
// }

int brightnessToTemperature(
    uint8_t brightness
) {

    // =============================
    // OFF handled separately
    // =============================

    if (brightness == 0) {

        return 24;
    }

    // =============================
    // Map 1-100 → 30-16
    // =============================

    int temp =
        map(
            brightness,
            1,
            100,
            30,
            16
        );

    return constrain(
        temp,
        16,
        30
    );
}

void alexaDeviceCallback(
    EspalexaDevice* dev
) {

    if (dev == nullptr) {

        return;
    }

    auto it =
        alexaDeviceMap.find(dev);

    if (
        it == alexaDeviceMap.end()
    ) {

        Logger::error(
            "ALEXA",
            "Unknown Alexa device"
        );

        return;
    }

    String deviceId =
        it->second;

    Device* device =
        DeviceService::getDeviceById(
            deviceId
        );

    if (device == nullptr) {

        Logger::error(
            "ALEXA",
            "Device not found"
        );

        return;
    }

    bool state =
        dev->getValue() > 0;

    Logger::info(
        "ALEXA",
        "%s -> %s",
        device->name.c_str(),
        state ? "ON" : "OFF"
    );


    if (
        device->type == DEVICE_AC)
    {
        uint8_t brightness =
        dev->getValue();

        ACState acState;

        acState.power =
            state;

        acState.temperature =
            brightnessToTemperature(
                brightness
            );

        acState.mode =
            ACMode::AC_MODE_COOL;

        acState.fan =
            AC_FAN_AUTO;

        acState.swing =
            false;

        bool success =
            ACControllerService::sendState(
                *device,
                acState);

        Logger::info(
            "ALEXA",
            success
                ? "AC state sent"
                : "Failed to send AC state");
    }
    else
    {

        ActionService::executeAction(
            device->id,
            state
                ? "power_on"
                : "power_off");
    }
}

void AlexaService::begin(AsyncWebServer& server) {

    std::vector<Device> devices = DeviceService::getDevices();
    for (const Device &device :devices)
    {

        EspalexaDevice *alexaDevice = new EspalexaDevice(device.name.c_str(), alexaDeviceCallback);

        espalexa.addDevice(alexaDevice);

        alexaDeviceMap[alexaDevice] = device.id;

        Logger::info("ALEXA","Registered Alexa device: %s",device.name.c_str());
    }

    server.onNotFound(

        [](AsyncWebServerRequest *request)
        {
            if (
                !AlexaService::handleRequest(
                    request))
            {

                request->send(
                    404,
                    "application/json",
                    "{\"success\":false,\"error\":\"Not found\"}");
            }
        });

    espalexa.begin(&server);

    Logger::info(
        "ALEXA",
        "ESPAlexa initialized"
    );
}

void AlexaService::loop() {

    espalexa.loop();
}

bool AlexaService::handleRequest(
    AsyncWebServerRequest* request
) {

    return espalexa.handleAlexaApiCall(
        request
    );
}

