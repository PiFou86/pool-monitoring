#pragma once

#include "ButtonAction.h"

class LCD2004Display;
class ButtonActionBacklightOn : public ButtonAction {
 private:
  LCD2004Display *m_lcd;

 public:
  ButtonActionBacklightOn(LCD2004Display *lcd);
  virtual void execute();
};
