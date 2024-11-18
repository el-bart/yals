#pragma once
#include "hardware/adc.h"
#include "pico/stdlib.h"

namespace Hal
{

struct Engine_current
{
  Engine_current()
  {
    adc_init();
    adc_gpio_init(pin);
  }

  Engine_current(Engine_current const&) = delete;
  Engine_current& operator=(Engine_current const&) = delete;
  Engine_current(Engine_current &&) = delete;
  Engine_current& operator=(Engine_current &&) = delete;

  float read_amps()
  {
    adc_select_input(adc_n);
    auto const sample = adc_read();

    constexpr float adc_conversion_factor = 4.3f / (1 << 12);   // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    auto const voltage = sample * adc_conversion_factor;

    constexpr float gain = 1.0f + 100.0f / 3.3f;    // gain on the OpAmp
    constexpr float r = 0.1f;                       // 0.1 Ohm measuring resistor
    auto const r_u = voltage / gain;                // voltage on the resistor
    auto const r_i = r_u / r;                       // resistor's current
    return r_i;
  }

private:
  static constexpr uint adc_n = 2;
  static constexpr uint pin = 26 + adc_n;
};

}
