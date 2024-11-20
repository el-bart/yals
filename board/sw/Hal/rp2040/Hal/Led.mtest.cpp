#include "Hal/Led.hpp"
#include <initializer_list>

int main()
{
  constexpr auto delay = 250;
  Hal::Led led;

  while (true)
    for(auto b=5u; b<256u; b+=25u)
    {
      led.brightness( static_cast<uint8_t>(b) );
      led.set(true);
      sleep_ms(delay);
      led.set(false);
      sleep_ms(delay);
    }
}
