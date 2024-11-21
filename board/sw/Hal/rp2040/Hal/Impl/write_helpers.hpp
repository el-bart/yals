#pragma once
#include "Hal/Uart.hpp"

namespace Hal::Impl
{

inline void write(Hal::Uart& uart, char const* str)
{
  for(; *str!=0; ++str)
    while(not uart.tx(*str))
    { }
}

inline void write_line(Hal::Uart& uart, char const* str)
{
  write(uart, str);
  write(uart, "\r\n");
}

template<typename ...Args>
void write_line_fmt(Hal::Uart& uart, char const* fmt, Args... args)
{
  char buf[1024];
  snprintf(buf, sizeof(buf), fmt, args...);
  write_line(uart, buf);
}

inline void purge_rx(Hal::Uart& uart)
{
  while( uart.ex() )
  { }
}

}
