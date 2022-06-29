#include "Display/LCD1602Display.h"

#include "Configuration/Configuration.h"
#include "Configuration.h"

#include <LiquidCrystal_I2C.h>

const size_t maxLines = 4;
const char inChar = 0b01111111;
const char outChar = 0b01111110;
const size_t lineCount = 4;
const size_t columnCount = 20;

LCD1602Display::LCD1602Display() : m_lcd(nullptr), m_waterPressure(NAN), m_waterInTemperature(NAN), m_waterOutTemperature(NAN), m_outdoorTemperature(NAN), m_outdoorHumidity(NAN), m_outdoorPressure(NAN), m_lastUpdate(0), m_currentLine(0), m_startedBacklightDate(0), m_backlightIsOn(false)
{
    this->m_lcd = new LiquidCrystal_I2C(0x27, columnCount, lineCount);
    this->m_lcd->begin(columnCount, lineCount);
    this->backlight();
    
}

void LCD1602Display::tick() {
    unsigned long currentDate = millis();

    if (currentDate - this->m_lastUpdate > VERTICAL_SCROLL_DELAY) {
        ++this->m_currentLine;

        if (this->m_currentLine > maxLines - lineCount) {
            this->m_currentLine = 0;
        }

        this->m_lcd->clear();
        this->m_lcd->setCursor(0, 0);
        this->m_lcd->println(getLine(this->m_currentLine));
        this->m_lcd->println(getLine(this->m_currentLine + 1));
        this->m_lcd->println(getLine(this->m_currentLine + 2));
        this->m_lcd->println(getLine(this->m_currentLine + 3));
    }

    if (this->m_backlightIsOn && currentDate - this->m_startedBacklightDate > BACKLIGHT_DELAY) {
        this->setBacklight(false);
    }
}

String LCD1602Display::getLine(const size_t &lineNumber) const {
    switch (lineNumber)
    {
    case 0:
        return String(this->m_waterPressure, 1) + " PSI";
        break;
    case 1:
        return String(inChar) + " " + String(this->m_waterInTemperature, 1) + " C " + String(outChar) + " " + String(this->m_waterOutTemperature, 1) + " C";
        break;
    case 2:
        return String("^ ") + String(this->m_outdoorTemperature, 1) + " C "
        + String(this->m_outdoorHumidity, 1) + " % "
        + String(this->m_outdoorPressure, 1) + " Pa" ;
        break;
    case 3:
        return "";
        break;
    
    default:
        break;
    }

    return "Undefined line information";
}

void LCD1602Display::backlight() {
    this->setBacklight(true);
}

void LCD1602Display::setBacklight(bool isOn) {
    this->m_lcd->setBacklight(isOn);
    this->m_backlightIsOn = isOn;
    this->m_startedBacklightDate = millis();
}