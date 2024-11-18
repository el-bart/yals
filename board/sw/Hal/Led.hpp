#pragma once
#include "pico/stdlib.h"
#include "hardware/pwm.h"

namespace Hal
{

struct Led
{
  Led():
    slice_num_{ pwm_gpio_to_slice_num(pin) }
  {
    gpio_set_function(pin, GPIO_FUNC_PWM);

    config_ = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&config_, PWM_DIV_FREE_RUNNING);
    constexpr auto pwm_freq = 10'000u;
    constexpr auto sys_clock_hz = SYS_CLK_KHZ * 1000u;
    constexpr auto max_value = 256u;
    constexpr auto cycles_per_run = sys_clock_hz / max_value;
    constexpr auto div = static_cast<float>(cycles_per_run) / pwm_freq;
    pwm_config_set_clkdiv(&config_, div);
    pwm_config_set_wrap(&config_, max_value);
    pwm_init(slice_num_, &config_, true);
  }

  Led(Led const&) = delete;
  Led& operator=(Led const&) = delete;
  Led(Led &&) = delete;
  Led& operator=(Led &&) = delete;

  void set(const bool status)
  {
    status_ = status;
    apply();
  }

  void toggle()
  {
    set(not status_);
  }

  void brightness(const uint8_t b)
  {
    brightness_ = b;
    apply();
  }

private:
  void apply() const
  {
    auto const b = status_ ? brightness_ : 0u;
    pwm_set_gpio_level(pin, b);
  }

  static constexpr uint pin = 4;

  const unsigned int slice_num_;
  pwm_config config_{};
  bool status_{false};
  uint8_t brightness_{255};
};

}
