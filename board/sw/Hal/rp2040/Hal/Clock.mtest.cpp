#include "Hal/Clock.hpp"
#include "Hal/Uart.hpp"
#include <cstdio>
#include "Utils/write_helpers.hpp"

using Utils::write_line;
using Utils::write_line_fmt;


int main()
{
  Hal::Clock clock;
  Hal::Uart uart;

  write_line(uart, ">>");
  write_line(uart, ">> HW Clock test app");
  write_line(uart, ">>");

  write_line_fmt(uart, ">> %llu ticks per second", clock.ticks_per_second().value_);
  Utils::purge_rx(uart);

  for(auto s=0u;; ++s)
  {
    auto const start = clock.now();
    write_line_fmt(uart, ">> at %us: %llu", s, start.value_);
#if 0
    sleep_ms(1'000);
#else
    auto const deadline = start + clock.ticks_per_second();
    while( clock.now() < deadline )
    { }
#endif
  }
}
