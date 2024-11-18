#include "hardware/uart.h"
#include "Hal/Uart.hpp"
#include <cstdio>

void write_long(Hal::Uart& uart, char const* str)
{
  auto left=str;
  while(*left!=0)
    left += uart.tx(left);
}

int main()
{
  Hal::Uart uart;

  write_long(uart,
      ">>\r\n"
      ">> HELLO USART!\r\n"
      ">>\r\n"
      ">> this text is too long to feet into a single\r\n"
      ">> write, this will be split. you should not be\r\n"
      ">> able to tell the difference.\r\n"
      ">>\r\n"
      );

  for(auto n=0u; ; ++n)
  {
    {
      char buf[64];
      snprintf(buf, sizeof(buf), "hello, serial #%u!\r\n", n);
      uart.tx(buf);
    }

    auto const oc = uart.rx();
    sleep_ms(500);
    if(not oc)
      continue;
    else
    {
      char buf[64];
      snprintf(buf, sizeof(buf), "got c=%d\r\n", int{*oc});
      uart.tx(buf);
    }
  }
}
