#pragma once
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include <optional>
#include <cstring>

namespace Hal::Impl
{

struct Uart
{
  Uart()
  {
    uart_init(uart_, baud);
    gpio_set_function(pin_tx, GPIO_FUNC_UART);
    gpio_set_function(pin_rx, GPIO_FUNC_UART);
  }

  Uart(Uart const&) = delete;
  Uart& operator=(Uart const&) = delete;
  Uart(Uart &&) = delete;
  Uart& operator=(Uart &&) = delete;

  bool tx(uint8_t b)
  {
    if(not uart_is_writable(uart_))
      return false;
    uart_putc(uart_, b);
    return true;
  }

  std::optional<uint8_t> rx()
  {
    if(not uart_is_readable(uart_))
      return {};
    return uart_getc(uart_);
  }

private:
  static constexpr uint pin_tx = 0;
  static constexpr uint pin_rx = 1;
  static constexpr unsigned int baud = 115200;
  decltype(uart0) uart_{uart0};
};

}
