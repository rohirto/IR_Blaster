#include "services/device_service.h"

#include <ArduinoJson.h>

#include "utils/logger.h"
#include "config/constants.h"
#include "config/system_config.h"
#include "services/storage_service.h"

std::vector<Device> DeviceService::devices;

void DeviceService::begin() {

    loadDevices();

    Logger::info(
        TAG_DEVICE,
        "Loaded %d devices",
        devices.size()
    );
}

std::vector<Device> DeviceService::getDevices() {

    return devices;
}

bool DeviceService::addDevice(
    const Device& device
)
{
    if (devices.size() >= MAX_DEVICES)
    {

        Logger::warn(
            TAG_DEVICE,
            "Maximum device limit reached");

        return false;
    }

    devices.push_back(device);

    saveDevices();

    return true;
}

void DeviceService::loadDevices() {

    devices.clear();

    if (!StorageService::exists(DEVICES_FILE)) {

        Logger::warn(
            TAG_DEVICE,
            "No devices.json found"
        );

        return;
    }

    String json = StorageService::readFile(
        DEVICES_FILE
    );
    // =================================
    // Validate Size
    // =================================

    if (
        json.length() >
        MAX_JSON_DOC_SIZE)
    {

        Logger::error(
            TAG_DEVICE,
            "Device file exceeds max size");

        return;
    }

    JsonDocument doc;

    Logger::debug(
            TAG_MEMORY,
            "Heap before deserialize: %u",
            ESP.getFreeHeap());
        
    DeserializationError error =
                deserializeJson(doc, json);

    Logger::debug(
            TAG_MEMORY,
            "Heap after deserialize: %u",
            ESP.getFreeHeap());


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
        DEVICES_FILE,
        output
    );
}

bool DeviceService::deleteDevice(
    const String& id
) {

    for (auto it = devices.begin(); it != devices.end(); ++it) {

        if (it->id == id) {

            Logger::info(
                TAG_DEVICE,
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
        TAG_DEVICE,
        "Device not found: %s",
        id.c_str()
    );

    return false;
}