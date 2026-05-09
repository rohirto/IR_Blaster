#pragma once

class SystemState {
public:

    // =================================
    // OTA State
    // =================================

    static bool isOtaInProgress();

    static void setOtaInProgress(
        bool value
    );

private:

    static bool otaInProgress;
};