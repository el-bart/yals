#pragma once
#include "Hal/All.hpp"

struct Context final
{
  struct Setpoints final
  {
    float min_pos_{};       // 0..100
    float max_pos_{};       // 0..100
    float position_{};      // 0..100
  };

  struct Last_reads final
  {
    float vcc_V_{};
    float engine_current_A_{};
    float position_percent_{};
  };

  Hal::All hal_;
  Setpoints setpoints_;
  Last_reads last_reads_;
};
