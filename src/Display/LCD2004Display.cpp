#include "Display/LCD2004Display.h"

#include <LiquidCrystal_I2C.h>

#include "Configuration.h"
#include "Configuration/Configuration.h"
#include "Log/Logger.h"

#include <math.h>

const size_t maxLines = 4;
const char inChar = 0b01111111;
const char outChar = 0b01111110;
const size_t lineCount = 4;
const size_t columnCount = 20;

LCD2004Display::LCD2004Display()
    : m_lcd(nullptr),
      m_waterPressure(NAN),
      m_waterInTemperature(NAN),
      m_waterOutTemperature(NAN),
      m_outdoorTemperature(NAN),
      m_outdoorHumidity(NAN),
      m_outdoorPressure(NAN),
      m_lastUpdate(0),
      m_currentLine(0),
      m_startedBacklightDate(0),
      m_backlightIsOn(false),
      m_isReady(false) {
  Logger.infoln("LCD2004Display::LCD2004Display()");
  // Logger.infoln("Creating LCD2004Display...");
  this->m_lcd = new LiquidCrystal_I2C(0x27, columnCount, lineCount);
  // Logger.infoln("begining LCD2004Display...");
  this->m_lcd->begin(columnCount, lineCount);
  // Logger.infoln("Setting LCD2004Display backlight...");
  this->backlight();
  this->clearMessages();
  this->ready();
}

void LCD2004Display::tick() {
  if (!this->isReady()) {
    return;
  }

  unsigned long currentDate = millis();

  if (currentDate - this->m_lastUpdate > VERTICAL_SCROLL_DELAY) {
    ++this->m_currentLine;

    if (this->m_currentLine > maxLines - lineCount) {
      this->m_currentLine = 0;
    }

    this->clearMessages();
    this->displayMessage(getLine(this->m_currentLine), false);
    this->displayMessage(getLine(this->m_currentLine + 1), false);
    this->displayMessage(getLine(this->m_currentLine + 2), false);
    this->displayMessage(getLine(this->m_currentLine + 3), false);

    this->m_lastUpdate = currentDate;
  }

  if (this->m_backlightIsOn &&
      currentDate - this->m_startedBacklightDate > BACKLIGHT_DELAY) {
    this->setBacklight(false);
  }
}

String LCD2004Display::getLine(const size_t &lineNumber) const {
  switch (lineNumber) {
    case 0:
      return String(this->m_waterPressure, 1) + " PSI";
      break;
    case 1:
      return String(inChar) + " " + String(this->m_waterInTemperature, 1) +
             " C " + String(outChar) + " " +
             String(this->m_waterOutTemperature, 1) + " C";
      break;
    case 2:
      return String("^ ") + String(this->m_outdoorTemperature, 1) + " C " +
             String(this->m_outdoorHumidity, 1) + " % " +
             String(this->m_outdoorPressure, 1) + " Pa";
      break;
    case 3:
      return "";
      break;

    default:
      break;
  }

  return "Undefined line information";
}

void LCD2004Display::backlight() { this->setBacklight(true); }

void LCD2004Display::setBacklight(bool isOn) {
  this->m_lcd->setBacklight(isOn);
  this->m_backlightIsOn = isOn;
  this->m_startedBacklightDate = millis();
}

void LCD2004Display::clearMessages() {
  this->m_messages[0] = "";
  this->m_messages[1] = "";
  this->m_messages[2] = "";
  this->m_messages[3] = "";
  this->m_messageIndex = -1;
}

void LCD2004Display::displayMessage(const String &message,
                                    const bool withBacklight) {
  // this->m_lcd->clear();
  // this->m_lcd->setCursor(0, 0);
  String messageToDisplay = message.substring(0, 20);

  if (this->m_messageIndex == 3) {
    this->m_messages[0] = this->m_messages[1];
    this->m_messages[1] = this->m_messages[2];
    this->m_messages[2] = this->m_messages[3];
    this->m_messages[3] = messageToDisplay;
  } else {
    this->m_messages[++this->m_messageIndex] = messageToDisplay;
  }

  this->m_lcd->clear();
  this->m_lcd->setCursor(0, 0);
  this->m_lcd->print(this->m_messages[0]);
  this->m_lcd->setCursor(0, 1);
  this->m_lcd->print(this->m_messages[1]);
  this->m_lcd->setCursor(0, 2);
  this->m_lcd->print(this->m_messages[2]);
  this->m_lcd->setCursor(0, 3);
  this->m_lcd->print(this->m_messages[3]);

  if (withBacklight) {
    this->backlight();
  }
}
