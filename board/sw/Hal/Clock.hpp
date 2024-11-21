#pragma once
#include "Hal/Impl/Clock.hpp"
#include "Utils/time.hpp"

namespace Hal
{

struct Clock
{
  Clock() = default;

  Utils::Timepoint now()      const { return impl_.now(); }
  uint64_t ticks_per_second() const { return impl_.ticks_per_second(); }

private:
  Impl::Clock impl_;
};

}
