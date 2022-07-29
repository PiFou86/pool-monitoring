#pragma once

#include "Display/LCD2004Display.h"
#include "MessageDisplay.h"

class LCD2004MessageDisplay : public MessageDisplay {
private:
    LCD2004Display *m_lcd2004Display;

public:
    inline LCD2004MessageDisplay(LCD2004Display *display) : m_lcd2004Display(display) {};
    inline void displayMessage(const String &message) {
        this->m_lcd2004Display->displayMessage(message);
    }
};
