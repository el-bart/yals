#include <initializer_list>
#include "pico/stdlib.h"

auto constexpr delay = 2'000;
constexpr uint ENG_IN_1 = 2;
constexpr uint ENG_IN_2 = 3;

int main()
{
  gpio_init(ENG_IN_1);
  gpio_set_dir(ENG_IN_1, GPIO_OUT);
  gpio_init(ENG_IN_2);
  gpio_set_dir(ENG_IN_2, GPIO_OUT);

  while (true)
    for(auto e1: {0, 1})
      for(auto e2: {0, 1})
      {
        gpio_put(ENG_IN_1, e1);
        gpio_put(ENG_IN_2, e2);
        sleep_ms(delay);
      }
}
