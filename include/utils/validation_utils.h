#pragma once

#include <Arduino.h>

class ValidationUtils {
public:

    static bool isValidId(
        const String& value
    ) {

        if (
            value.isEmpty() ||
            value.length() > 64
        ) {

            return false;
        }

        for (char c : value) {

            bool valid =
                isalnum(c) ||
                c == '_' ||
                c == '-';

            if (!valid) {

                return false;
            }
        }

        return true;
    }

    static bool isValidName(
        const String& value
    ) {

        if (
            value.isEmpty() ||
            value.length() > 64
        ) {

            return false;
        }

        return true;
    }
    static bool isValidAction(
        const String &value)
    {

        if (
            value.isEmpty() ||
            value.length() > 64)
        {

            return false;
        }

        for (char c : value)
        {

            bool valid =
                isalnum(c) ||
                c == '_' ||
                c == '-';

            if (!valid)
            {

                return false;
            }
        }

        return true;
    }
};