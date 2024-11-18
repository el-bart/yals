#include "Hal/Vcc_feedback.hpp"
#include "Hal/Uart.hpp"
#include <cstdio>

int main()
{
  Hal::Vcc_feedback vccf;
  Hal::Uart uart;

  uart.tx("ADC test for Vcc\r\n");

  while(true)
  {
    const auto vcc = vccf.volts();
    char buf[32];
    snprintf(buf, sizeof(buf), "V=%f\r\n", vcc);
    uart.tx(buf);
    sleep_ms(500);
  }
}
