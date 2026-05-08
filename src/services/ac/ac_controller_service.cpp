#include "services/ac/ac_controller_service.h"

#include "utils/logger.h"

#include "services/ac/hitachi_ac_service.h"

bool ACControllerService::sendState(
    const Device& device,
    const ACState& state
) {

    Logger::info(
        "AC_CTRL",
        "Sending AC state for device '%s'",
        device.id.c_str()
    );

    switch (device.acBrand) {

        // =============================
        // Hitachi
        // =============================

        case AC_BRAND_HITACHI:

            return HitachiAcService::sendState(
                state
            );

        // =============================
        // Unsupported
        // =============================

        default:

            Logger::error(
                "AC_CTRL",
                "Unsupported AC brand"
            );

            return false;
    }
}