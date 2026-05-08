#include "models/device.h"

Device::Device() {
}

Device::Device(
    String id,
    String name,
    DeviceType type,
    ACBrand acBrand
) {

    this->id = id;

    this->name = name;

    this->type = type;

    this->acBrand = acBrand;
}