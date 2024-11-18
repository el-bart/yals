#pragma once
#include "Hal/Impl/Led.hpp"

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
