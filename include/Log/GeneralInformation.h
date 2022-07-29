#pragma once

#include <Arduino.h>
#include "MessageDisplay.h"

class GeneralInformationImpl {
 private:
  MessageDisplay *m_messageDisplay;

 public:
  inline GeneralInformationImpl() : m_messageDisplay(nullptr){};
  inline void setMessageDisplay(MessageDisplay *messageDisplay) {
    this->m_messageDisplay = messageDisplay;
  }
  inline void displayMessage(const String &message) {
    if (this->m_messageDisplay != nullptr) {
      this->m_messageDisplay->displayMessage(message);
    }
  }
};

extern GeneralInformationImpl GeneralInformation;