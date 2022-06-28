#pragma once

#include "ButtonAction.h"

class ButtonActionNull : public ButtonAction {
 public:
  inline virtual void execute() { ; }
};