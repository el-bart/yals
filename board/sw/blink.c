#include "pico/stdlib.h"

int main()
{
  uint pin_e1 = 2;
  uint pin_e2 = 3;
  uint pin_led = 4;
  uint pins[3] = { pin_e1, pin_e2, pin_led };

  for(int i=0; i<3; ++i)
  {
    gpio_init(pins[i]);
    gpio_set_dir(pins[i], GPIO_OUT);
  }

  int delay = 500;
  int state = 0x00;
  int flip = 0x01;  // flip state bit

  while (true)
  {
    state ^= flip;
    gpio_put(pin_led, state);
    gpio_put(pin_e1, state);
    gpio_put(pin_e2, state ^ flip);
    sleep_ms(delay);
  }
}
