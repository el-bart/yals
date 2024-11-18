#pragma once
#include "hardware/pwm.h"
#include "pico/stdlib.h"

namespace Hal
{

struct Engine
{
  enum Direction
  {
    Off,
    Left,
    Right
  };

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

  void direction(const Direction dir)
  {
    dir_ = dir;
    apply();
  }

  void force(const uint16_t f)
  {
    force_ = f;
    apply();
  }

  void set(const Direction dir, const uint16_t f)
  {
    dir_ = dir;
    force_ = f;
    apply();
  }

private:
  void apply() const
  {
    switch(dir_)
    {
      case Direction::Off:   return pwm_set_both_levels(slice_num_, 0,      0     );
      case Direction::Left:  return pwm_set_both_levels(slice_num_, 0,      force_);
      case Direction::Right: return pwm_set_both_levels(slice_num_, force_, 0     );
    }
  }


  static constexpr uint pin_pos = 2;    // out 1 == ENG+
  static constexpr uint pin_neg = 3;    // out 2 == ENG-

  pwm_config config_{};
  unsigned int slice_num_{};
  Direction dir_{Direction::Off};
  uint16_t force_{0};
};

}
