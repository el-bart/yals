#pragma once
#include "pico/stdlib.h"

namespace Hal
{

struct Led
{
  Led()
  {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
  }

  Led(Led const&) = delete;
  Led& operator=(Led const&) = delete;
  Led(Led &&) = delete;
  Led& operator=(Led &&) = delete;

  void set(const bool status)
  {
    status_ = status;
    gpio_put(pin, status_ ? 1: 0);
  }

  void toggle()
  {
    set(not status_);
  }

  void brightness(uint8_t b)
  {
    brightness_ = b;
  }

private:
  static constexpr uint pin = 4;

  bool status_{false};
  uint8_t brightness_{255};
};

}
