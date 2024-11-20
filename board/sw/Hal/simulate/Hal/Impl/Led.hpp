#pragma once
#include "Hal/Sim.hpp"

namespace Hal::Impl
{

struct Led
{
  Led() = default;

  Led(Led const&) = delete;
  Led& operator=(Led const&) = delete;
  Led(Led &&) = delete;
  Led& operator=(Led &&) = delete;

  void apply(bool status, uint8_t brightness) const
  {
    if(not status)
      sim().LED_brightness_ = 0;
    else
      sim().LED_brightness_ = brightness / 255.0f;
  }
};

}
