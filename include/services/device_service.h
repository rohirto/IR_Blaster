#pragma once

#include <vector>

#include "models/device.h"

class DeviceService {
public:

    static void begin();

    static std::vector<Device> getDevices();

    static bool addDevice(
        const Device& device
    );

    static bool deleteDevice(
        const String& id
    );

private:

    static std::vector<Device> devices;

    static void loadDevices();

    static void saveDevices();
};