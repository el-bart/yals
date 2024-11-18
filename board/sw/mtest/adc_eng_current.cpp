#include "Hal/Engine.hpp"
#include "Hal/Engine_current.hpp"
#include "Hal/Uart.hpp"
#include <cstdio>

int main()
{
  Hal::Engine_current ec;
  Hal::Engine eng;
  Hal::Uart uart;

  uart.tx("ADC test for engine current\r\n");
  eng.set(Hal::Engine::Direction::Left, 65535u);

  while(true)
  {
    const auto amps = ec.amps();

    char buf[32];
    snprintf(buf, sizeof(buf), "I=%f\r\n", amps);
    uart.tx(buf);

    sleep_ms(500);
  }
}
