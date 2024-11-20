#pragma once
#include "Hal/All.hpp"

struct Context final
{
  struct Setpoints final
  {
    // TODO: min/max pos shall be read from EEPROM
    float min_pos_{0};      // 0..1
    float max_pos_{100};    // 0..1
    float position_{};      // 0..1
  };

  struct Last_reads final
  {
    float vcc_V_{};
    float engine_current_A_{};
    float position_{};      // 0..1
  };

  Hal::All hal_;
  Setpoints setpoints_;
  Last_reads last_reads_;
};
