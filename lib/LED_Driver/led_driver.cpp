// ===============================================
// File: lib/LedDriver/LedDriver.cpp
// ===============================================

#include "led_driver.h"

LedDriver::LedDriver(uint8_t pin, bool activeLow)
    : _pin(pin),
      _activeLow(activeLow),
      _state(false),
      _blinking(false),
      _blinkInterval(500),
      _lastBlinkTime(0) {}

void LedDriver::begin() {
    pinMode(_pin, OUTPUT);
    off();
}

void LedDriver::writePin(bool state) {
    if (_activeLow) {
        digitalWrite(_pin, state ? LOW : HIGH);
    } else {
        digitalWrite(_pin, state ? HIGH : LOW);
    }
}

void LedDriver::on() {
    _blinking = false;
    _state = true;
    writePin(true);
}

void LedDriver::off() {
    _blinking = false;
    _state = false;
    writePin(false);
}

void LedDriver::toggle() {
    _state = !_state;
    writePin(_state);
}

void LedDriver::set(bool state) {
    if (state) {
        on();
    } else {
        off();
    }
}

bool LedDriver::getState() const {
    return _state;
}

void LedDriver::blink(uint32_t intervalMs) {
    _blinking = true;
    _blinkInterval = intervalMs;
    _lastBlinkTime = millis();
}

void LedDriver::stopBlink() {
    _blinking = false;
}

void LedDriver::update() {
    if (!_blinking) {
        return;
    }

    uint32_t now = millis();

    if (now - _lastBlinkTime >= _blinkInterval) {
        _lastBlinkTime = now;
        toggle();
    }
}