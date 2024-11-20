#pragma once
#include "Hal/Impl/Led.hpp"
#include <cmath>

namespace Hal
{

struct Led
{
  Led() = default;

  void set(const bool status)
  {
    status_ = status;
    apply();
  }

  void toggle()
  {
    set(not status_);
  }

  void brightness(const float b)
  {
    brightness( static_cast<uint8_t>( roundf(b * 255) ) );
  }

  void brightness(const uint8_t b)
  {
    brightness_ = b;
    apply();
  }

private:
  void apply() const
  {
    impl_.apply(status_, brightness_);
  }

  bool status_{false};
  uint8_t brightness_{255};
  Impl::Led impl_;
};

}
