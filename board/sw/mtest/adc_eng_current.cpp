#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/adc.h"

// ADC 1:
constexpr auto ADC_N = 2;
constexpr auto ADC_PIN = 26 + ADC_N;

// load:
constexpr uint ENG_IN_1 = 2;
constexpr uint ENG_IN_2 = 3;

int main()
{
  adc_init();
  adc_gpio_init(ADC_PIN);
  adc_select_input(ADC_N);

  gpio_init(ENG_IN_1);
  gpio_set_dir(ENG_IN_1, GPIO_OUT);
  gpio_init(ENG_IN_2);
  gpio_set_dir(ENG_IN_2, GPIO_OUT);
  // make engine spit, to allow for artificial load
  gpio_put(ENG_IN_1, 0);
  gpio_put(ENG_IN_2, 1);

  auto uart = uart0;
  constexpr auto uart_pin_tx = 0;
  constexpr auto uart_pin_rx = 1;
  constexpr auto uart_baud = 115200;
  uart_init(uart, uart_baud);
  gpio_set_function(uart_pin_tx, GPIO_FUNC_UART);
  gpio_set_function(uart_pin_rx, GPIO_FUNC_UART);
  stdio_init_all();
  printf("ADC test for engine current\n");

  while(true)
  {
    auto const v = adc_read();

    constexpr float adc_conversion_factor = 3.3f / (1 << 12);   // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    auto const f = v * adc_conversion_factor;

    constexpr float gain = 1.0f + 100.0f / 3.3f;    // gain on the OpAmp
    constexpr float r = 0.1f;                       // 0.1 Ohm measuring resistor
    auto const r_v = f / gain;                      // voltage on the resistor
    auto const r_i = r_v / r;                       // resistor's current

    printf("u=%u f=%f Vr=%f I=%f\n", v, f, r_v, r_i);
    sleep_ms(500);
  }
}
