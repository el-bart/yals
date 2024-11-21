#pragma once
#include "Hal/All.hpp"
#include "Io/Buffer.hpp"
#include "Utils/Config/settings.hpp"

struct Context final
{
  struct Setpoints final
  {
    float LED_brightness_{Utils::Config::default_LED_brightness}; // 0..1
    float min_pos_{0.0};        // 0..1
    float max_pos_{1.0};        // 0..1
    float position_{};          // 0..1
  };

  struct Last_reads final
  {
    float vcc_V_{};
    float engine_current_A_{};
    float position_{};          // 0..1
  };

  Hal::All hal_;
  Setpoints setpoints_;
  Last_reads last_reads_;

  Io::Buffer tx_buffer_;
  Io::Buffer rx_buffer_;
};
