#pragma once
#include "Utils/time.hpp"
#include <pico/time.h>

namespace Hal::Impl
{

struct Clock
{
  Clock() = default;

  Clock(Clock const&) = delete;
  Clock& operator=(Clock const&) = delete;
  Clock(Clock&&) = delete;
  Clock& operator=(Clock&&) = delete;

  Utils::Timepoint now() const
  {
    return { .value_ = value( get_absolute_time() ) };
  }

  uint64_t ticks_per_second() const { return ticks_per_second_; }

private:
  static uint64_t compute_ticks_per_second()
  {
    auto const now = get_absolute_time();
    auto const later = delayed_by_ms(now, 1'000);
    return value(later) - value(now);
  }

  static uint64_t value(absolute_time_t const att)
  {
    // that's how it's defined in Pico's SDK...
#ifdef NDEBUG
    return att;
#else
    return att._private_us_since_boot;
#endif
  }

  uint64_t const ticks_per_second_{ compute_ticks_per_second() };
};

}
