#include "Hal/Clock.hpp"
#include "Hal/Uart.hpp"
#include <cstdio>

int main()
{
  Hal::Clock clock;
  Hal::Uart uart;

  uart.tx("\r\n>> HW Clock test app\r\n");

  {
    char buf[32];
    snprintf(buf, sizeof(buf), ">> %llu ticks per second\r\n", clock.ticks_per_second());
    uart.tx(buf);
  }

  while(true)
  {
    char buf[32];
    snprintf(buf, sizeof(buf), ">> at: %llu\r\n", clock.now().value_);
    uart.tx(buf);
    sleep_ms(500);
  }
}
