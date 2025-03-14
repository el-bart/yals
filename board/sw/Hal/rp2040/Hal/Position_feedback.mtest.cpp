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
    auto const value = pf.value();
    char buf[16];
    snprintf(buf, sizeof(buf), "p=%f%%\r\n", value * 100.0f);
    uart.tx(buf);
    sleep_ms(100);
  }
}
