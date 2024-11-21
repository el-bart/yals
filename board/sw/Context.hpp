#pragma once
#include "Hal/All.hpp"
#include "Utils/Config/settings.hpp"

struct Context final
{
  struct Setpoints final
  {
    float LED_brightness_{Utils::Config::default_LED_brightness};   // 0..1
    float min_pos_{Utils::Config::servo_absolute_min};              // 0..1
    float max_pos_{Utils::Config::servo_absolute_max};              // 0..1
    float position_{};                                              // 0..1
  };

  struct Last_reads final
  {
    float vcc_V_{};             // [V]
    float engine_current_A_{};  // [A]
    float position_{};          // 0..1
  };

  Hal::All hal_;
  Setpoints setpoints_;
  Last_reads last_reads_;
};
