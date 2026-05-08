#include "utils/ir_utils.h"

bool IRUtils::isSupportedProtocol(
    const String& protocol
) {

    return (
        protocol == "NEC" ||
        protocol == "SONY" ||
        protocol == "SAMSUNG"
    );
}