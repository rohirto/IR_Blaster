#include "models/device.h"

Device::Device() {
}

Device::Device(
    String id,
    String name,
    DeviceType type
) {

    this->id = id;

    this->name = name;

    this->type = type;
}