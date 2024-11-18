#pragma once
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <initializer_list>

namespace Hal::Impl
{

struct Engine
{
  Engine():
    slice_num_{ pwm_gpio_to_slice_num(pin_pos) }
  {
    // make sure both channels are on the same slice!
    static_assert( pin_pos + 1 == pin_neg );
    static_assert( pin_neg % 2 == 1 );
    assert( pwm_gpio_to_slice_num(pin_pos) == pwm_gpio_to_slice_num(pin_neg) );

    // PWM setup
    for(auto pin: {pin_pos, pin_neg})
      gpio_set_function(pin, GPIO_FUNC_PWM);

    config_ = pwm_get_default_config();
    // make sure PWM frequency is ~1-2kHz
    constexpr auto sys_clock_hz = SYS_CLK_KHZ * 1000;
    static_assert( sys_clock_hz / 65536 >= 1000 );
    static_assert( sys_clock_hz / 65536 <= 2000 );
    pwm_config_set_clkdiv(&config_, 1.0f);
    pwm_config_set_clkdiv_mode(&config_, PWM_DIV_FREE_RUNNING);
    pwm_init(slice_num_, &config_, true);
  }

  Engine(Engine const&) = delete;
  Engine& operator=(Engine const&) = delete;
  Engine(Engine &&) = delete;
  Engine& operator=(Engine &&) = delete;

  void apply(int dir, uint16_t force) const
  {
    if(dir==0)
      return pwm_set_both_levels(slice_num_, 0, 0);
    if(dir<0)
      return pwm_set_both_levels(slice_num_, force, 0);
    if(dir>0)
      return pwm_set_both_levels(slice_num_, 0, force);
  }

private:
  static constexpr uint pin_pos = 2;    // out 1 == ENG+
  static constexpr uint pin_neg = 3;    // out 2 == ENG-

  pwm_config config_{};
  unsigned int slice_num_{};
};

}
