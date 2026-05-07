#pragma once

enum SystemMode {

    NORMAL_MODE = 0,

    CONFIG_MODE = 1
};

class ModeService {
public:

    static void begin();

    static void setMode(
        SystemMode mode
    );

    static SystemMode getMode();

    static bool isNormalMode();

    static bool isConfigMode();

    static const char* getModeString();

private:

    static SystemMode currentMode;
};