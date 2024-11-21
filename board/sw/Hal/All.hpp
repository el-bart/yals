#pragma once
#include "Hal/Engine.hpp"
#include "Hal/Engine_current.hpp"
#include "Hal/Led.hpp"
#include "Hal/Position_feedback.hpp"
#include "Hal/Uart.hpp"
#include "Hal/Vcc_feedback.hpp"
#include "Hal/EEPROM.hpp"
#include "Hal/Clock.hpp"

namespace Hal
{

struct All
{
  Led led_;
  Uart uart_;
  Engine eng_;
  Engine_current eng_current_;
  Position_feedback pos_;
  Vcc_feedback vcc_;
  EEPROM EEPROM_;
  Clock clock_;
};

}
