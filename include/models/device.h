#pragma once

#include <Arduino.h>
#include <vector>

enum DeviceType {
    DEVICE_TV,
    DEVICE_AC,
    DEVICE_FAN,
    DEVICE_LIGHT,
    DEVICE_CUSTOM
};

class Device {
public:

    String id;

    String name;

    DeviceType type;

    Device();

    Device(
        String id,
        String name,
        DeviceType type
    );
};