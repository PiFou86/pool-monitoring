#pragma once

#include "ButtonAction.h"

class LCD1602Display;
class ButtonActionBacklightOn : public ButtonAction {
 private:
  LCD1602Display *m_lcd;

 public:
  ButtonActionBacklightOn(LCD1602Display *lcd);
  virtual void execute();
};
