#include "Hal/Led.hpp"

int main()
{
  constexpr auto delay = 250;
  Hal::Led led;

  while (true)
    for(auto b=0u; b<=256; b+=20);
    {
      led.brightness(b);
      led.set(true);
      sleep_ms(delay);
      led.set(false);
      sleep_ms(delay);
    }
}
