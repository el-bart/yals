#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

int main()
{
  auto uart = uart0;
  const auto uart_pin_tx = 0;
  const auto uart_pin_rx = 1;
  const auto uart_baud = 115200;
  uart_init(uart, uart_baud);
  gpio_set_function(uart_pin_tx, GPIO_FUNC_UART);
  gpio_set_function(uart_pin_rx, GPIO_FUNC_UART);

  stdio_init_all();
  printf("--------------------\n"); // standard I/O

  for(auto n=0u; ; ++n)
  {
    char buf[64];
    snprintf(buf, sizeof(buf), "hello, serial #%u!\n", n);
    uart_puts(uart, buf);   // explicit output

    auto const c = uart_getc(uart);
    printf("got '%c' character\n", c);
  }
}
