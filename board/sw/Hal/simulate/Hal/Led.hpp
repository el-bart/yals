#pragma once
#include "Hal/Sim.hpp"

namespace Hal
{

struct Led
{
  Led() = default;

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
    if(not status_)
      sim().led_brightness_ = 0;
    else
      sim().led_brightness_ = brightness_ / 255.0f;
  }

  bool status_{false};
  uint8_t brightness_{255};
};

}
