#pragma once
#include "Hal/Impl/Watchdog.hpp"

namespace Hal
{

struct Watchdog
{
  Watchdog() = default;
  void reset() { impl_.reset(); }
  static bool rebooted_by_watchdog() { return Impl::Watchdog::rebooted_by_watchdog(); }

private:
  Impl::Watchdog impl_;
};

}
