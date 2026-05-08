#pragma once

#include <Arduino.h>
#include <vector>

#include "models/ac_brand.h"

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

    ACBrand acBrand = AC_BRAND_NONE;

    Device();

    Device(
        String id,
        String name,
        DeviceType type,
        ACBrand acBrand = AC_BRAND_NONE
    );
};