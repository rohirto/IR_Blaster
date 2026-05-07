#include "services/device_service.h"

#include <ArduinoJson.h>

#include "logger.h"
#include "services/storage_service.h"

std::vector<Device> DeviceService::devices;

void DeviceService::begin() {

    loadDevices();

    Logger::info(
        "DEVICE",
        "Loaded %d devices",
        devices.size()
    );
}

std::vector<Device> DeviceService::getDevices() {

    return devices;
}

bool DeviceService::addDevice(
    const Device& device
) {

    devices.push_back(device);

    saveDevices();

    return true;
}

void DeviceService::loadDevices() {

    devices.clear();

    if (!StorageService::exists("/devices.json")) {

        Logger::warn(
            "DEVICE",
            "No devices.json found"
        );

        return;
    }

    String json = StorageService::readFile(
        "/devices.json"
    );

    JsonDocument doc;

    deserializeJson(doc, json);

    JsonArray array = doc.as<JsonArray>();

    for (JsonObject obj : array) {

        Device device(
            obj["id"].as<String>(),
            obj["name"].as<String>(),
            (DeviceType)obj["type"].as<int>()
        );

        devices.push_back(device);
    }
}

void DeviceService::saveDevices() {

    JsonDocument doc;

    JsonArray array = doc.to<JsonArray>();

    for (Device& device : devices) {

        JsonObject obj = array.add<JsonObject>();

        obj["id"] = device.id;

        obj["name"] = device.name;

        obj["type"] = device.type;
    }

    String output;

    serializeJsonPretty(doc, output);

    StorageService::writeFile(
        "/devices.json",
        output
    );
}

bool DeviceService::deleteDevice(
    const String& id
) {

    for (auto it = devices.begin(); it != devices.end(); ++it) {

        if (it->id == id) {

            Logger::info(
                "DEVICE",
                "Deleting device: %s",
                id.c_str()
            );

            devices.erase(it);

            saveDevices();

            // Future:
            // delete associated command file
            // Example:
            // /commands/<device_id>.json

            return true;
        }
    }

    Logger::warn(
        "DEVICE",
        "Device not found: %s",
        id.c_str()
    );

    return false;
}