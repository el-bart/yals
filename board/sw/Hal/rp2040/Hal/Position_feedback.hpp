#pragma once
#include "hardware/adc.h"
#include "pico/stdlib.h"

namespace Hal
{

struct Position_feedback
{
  Position_feedback()
  {
    adc_init();
    adc_gpio_init(pin);
  }

  Position_feedback(Position_feedback const&) = delete;
  Position_feedback& operator=(Position_feedback const&) = delete;
  Position_feedback(Position_feedback &&) = delete;
  Position_feedback& operator=(Position_feedback &&) = delete;

  float percent()
  {
    adc_select_input(adc_n);
    auto const sample = adc_read();

    constexpr float adc_conversion_factor = 3.3f / (1 << 12); // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    auto const voltage = sample * adc_conversion_factor;
    auto const percent = voltage / 3.3f * 100.0f;
    return percent;
  }

private:
  static constexpr uint adc_n = 0;
  static constexpr uint pin = 26 + adc_n;
};

}
