#pragma once

#include <Arduino.h>

class MessageDisplay {

public:
    virtual void displayMessage(const String &message) = 0;
};
