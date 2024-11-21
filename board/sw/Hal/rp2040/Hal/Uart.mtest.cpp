#include "Hal/Uart.hpp"
#include <cstdio>
#include "Hal/Impl/write_helpers.hpp"

using Hal::Impl::write_line;
using Hal::Impl::write_line_fmt;


int main()
{
  Hal::Uart uart;

  write_line(uart, ">>");
  write_line(uart, ">> HELLO USART!");
  write_line(uart, ">>");
  write_line(uart, ">> this text is too long to feet into a single");
  write_line(uart, ">> write, this will be split. you should not be");
  write_line(uart, ">> able to tell the difference.");
  write_line(uart, ">>");

  for(auto n=0u; ; ++n)
  {
    write_line_fmt(uart, "hello, serial #%u!\r\n", n);

    auto const oc = uart.rx();
    sleep_ms(500);
    if(not oc)
      continue;
    else
      write_line_fmt(uart, "got c=%d\r\n", int{*oc});
  }
}
