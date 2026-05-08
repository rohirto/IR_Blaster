#pragma once

#include <Arduino.h>

enum LearnState {

    LEARN_IDLE = 0,

    LEARN_WAITING,

    LEARN_SUCCESS,

    LEARN_FAILED
};

class LearnService {
public:

    static void begin();

    static void loop();

    static bool startLearning(
        const String& deviceId,
        const String& action
    );

    static void cancelLearning();

    static LearnState getState();

    static const char* getStateString();

    static String getCurrentDeviceId();

    static String getCurrentAction();

private:

    static LearnState currentState;

    static String currentDeviceId;

    static String currentAction;

    static unsigned long learnStartTime;
};