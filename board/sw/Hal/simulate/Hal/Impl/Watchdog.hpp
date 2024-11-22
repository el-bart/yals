#pragma once
#include "Hal/Impl/Watchdog.hpp"
#include "Hal/Sim.hpp"

namespace Hal::Impl
{

struct Watchdog
{
  Watchdog() = default;
  void reset() { sim().last_watchdog_reset_time_ = sim().current_time_; }
  static bool rebooted_by_watchdog() { return false; }
};

}
