#pragma once
#include "Hal/Sim.hpp"
#include "Utils/time.hpp"

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
    return { .value_ = sim().current_time_ };
  }

  Utils::Ticks ticks_per_second() const
  {
    return { .value_ = Sim::ticks_per_second_ };
  }
};

}
