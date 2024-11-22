#pragma once
#include "Hal/Impl/Watchdog.hpp"

namespace Hal
{

struct Watchdog
{
  Watchdog() = default;
  void reset() { impl_.reset(); }

private:
  Impl::Watchdog impl_;
};

}
