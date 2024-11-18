#pragma once
#include "hardware/adc.h"
#include "pico/stdlib.h"

namespace Hal::Impl
{

struct Vcc_feedback
{
  Vcc_feedback()
  {
    adc_init();
    adc_gpio_init(pin);
  }

  Vcc_feedback(Vcc_feedback const&) = delete;
  Vcc_feedback& operator=(Vcc_feedback const&) = delete;
  Vcc_feedback(Vcc_feedback &&) = delete;
  Vcc_feedback& operator=(Vcc_feedback &&) = delete;

  float volts()
  {
    adc_select_input(adc_n);
    auto const sample = adc_read();

    constexpr float adc_conversion_factor = 3.3f / (1 << 12);       // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    constexpr float div_conversion_factor = 1.0f + 100.0f / 22.0f;  // resistor divider: Vcc -> 100k -> ADC -> 22k -> GND
    auto const div_volts = sample * adc_conversion_factor;
    auto const vcc = div_volts * div_conversion_factor;
    return vcc;
  }

private:
  static constexpr uint adc_n = 1;
  static constexpr uint pin = 26 + adc_n;
};

}
