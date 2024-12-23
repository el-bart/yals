#pragma once
#include "hardware/adc.h"
#include "pico/stdlib.h"

namespace Hal::Impl
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

  float value()
  {
    adc_select_input(adc_n);
    auto const sample = adc_read();

    constexpr float adc_conversion_factor = 3.3f / (1 << 12); // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    auto const voltage = sample * adc_conversion_factor;
    auto const value = voltage / 3.3f;
    return 1.0f - value;    // value has to be inverted, due to potentiometer's direction
  }

private:
  static constexpr uint adc_n = 0;
  static constexpr uint pin = 26 + adc_n;
};

}
