#pragma once
#include <hardware/watchdog.h>
#include "Utils/Config/settings.hpp"
#include "Hal/Impl/EEPROM.hpp"

namespace Hal::Impl
{

struct Watchdog
{
  Watchdog()
  {
    using namespace Utils::Config;

    auto constexpr MHzs = XOSC_KHZ / 1'000u;
    watchdog_start_tick(MHzs);

    auto constexpr control_loop_time_ms = control_loop_time_s * 1'000u;
    auto constexpr value_write_time_ms = sizeof(EEPROM::value_type) * EEPROM::delay_to_write_data_ms;
    auto constexpr delay_ms = watchdog_allow_cycles_skip * control_loop_time_ms;
    static_assert( delay_ms > value_write_time_ms + 2*control_loop_time_ms );
    auto constexpr pause_on_debug = false;
    watchdog_enable(delay_ms, pause_on_debug);
  }

  Watchdog(Watchdog const&) = delete;
  Watchdog& operator=(Watchdog const&) = delete;
  Watchdog(Watchdog&&) = delete;
  Watchdog& operator=(Watchdog&&) = delete;

  void reset() { watchdog_update(); }
  static bool rebooted_by_watchdog() { return watchdog_caused_reboot(); }
};

}
