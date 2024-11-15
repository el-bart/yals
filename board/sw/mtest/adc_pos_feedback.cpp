#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/adc.h"

// ADC 1:
constexpr auto ADC_N = 0;
constexpr auto ADC_PIN = 26 + ADC_N;

int main()
{
  adc_init();
  adc_gpio_init(ADC_PIN);
  adc_select_input(ADC_N);

  auto uart = uart0;
  constexpr auto uart_pin_tx = 0;
  constexpr auto uart_pin_rx = 1;
  constexpr auto uart_baud = 115200;
  uart_init(uart, uart_baud);
  gpio_set_function(uart_pin_tx, GPIO_FUNC_UART);
  gpio_set_function(uart_pin_rx, GPIO_FUNC_UART);
  stdio_init_all();
  printf("ADC test for position feedback\n");

  while(true)
  {
    auto const v = adc_read();
    constexpr float adc_conversion_factor = 3.3f / (1 << 12); // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    auto const f = v * adc_conversion_factor;
    auto const percent = f / 3.3f * 100.0f;
    printf("u=%u V=%f p=%f%%\n", v, f, percent);
    sleep_ms(500);
  }
}
