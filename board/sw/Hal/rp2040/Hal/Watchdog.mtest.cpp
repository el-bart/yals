#include "Hal/Watchdog.hpp"
#include "Hal/Uart.hpp"
#include "Utils/Config/settings.hpp"
#include "Utils/write_helpers.hpp"

using Utils::write_line;
using Utils::write_line_fmt;

int main()
{
  Hal::Uart uart;

  write_line(uart, ">>");
  write_line(uart, ">> watchdog app");
  write_line(uart, ">>");
  Utils::purge_rx(uart);

  if( watchdog_caused_reboot() )
  {
    write_line(uart, ">> REBOOT WAS CAUSED BY WATCHDOG!");
    write_line(uart, ">> STOPPING APP NOW TO PREVENT BUSY LOOP (YOU ARE WELCOME)");
    while(true)
      if(auto const c = uart.rx(); c)
        write_line(uart, ">> yes, i'm still here");
  }

  Hal::Watchdog watchdog;

  for(auto n=0u; true; ++n)
  {
    write_line_fmt(uart, ">> cycle length %u[ms]", n);
    sleep_ms(n);
    watchdog.reset();
  }
}
