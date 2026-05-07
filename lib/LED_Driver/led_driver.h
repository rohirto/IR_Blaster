// ===============================================
// File: lib/LedDriver/LedDriver.h
// ===============================================

#pragma once

#include <Arduino.h>

class LedDriver {
public:
    LedDriver(uint8_t pin, bool activeLow = true);

    void begin();

    void on();
    void off();
    void toggle();

    void set(bool state);
    bool getState() const;

    void blink(uint32_t intervalMs);
    void stopBlink();

    void update();

private:
    uint8_t _pin;
    bool _activeLow;

    bool _state;

    bool _blinking;
    uint32_t _blinkInterval;
    uint32_t _lastBlinkTime;

    void writePin(bool state);
};