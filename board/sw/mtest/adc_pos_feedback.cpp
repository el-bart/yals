#include "Hal/Position_feedback.hpp"
#include "Hal/Uart.hpp"
#include <cstdio>

int main()
{
  Hal::Position_feedback pf;
  Hal::Uart uart;

  uart.tx("ADC test for position feedback\r\n");

  while(true)
  {
    auto const percent = pf.read_percent();
    char buf[16];
    snprintf(buf, sizeof(buf), "p=%f%%\r\n", percent);
    uart.tx(buf);
    sleep_ms(500);
  }
}
