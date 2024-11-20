#include "Hal/Impl/EEPROM.hpp"
#include "Hal/Uart.hpp"
#include <algorithm>
#include <cstdio>

void write(Hal::Uart& uart, char const* str)
{
  for(; *str!=0; ++str)
    while(not uart.tx(*str))
    { }
}

void write_line(Hal::Uart& uart, char const* str)
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

int main()
{
  Hal::Impl::EEPROM eeprom;
  Hal::Uart uart;

  write_line(uart, "");
  write_line(uart, "");
  write_line(uart, ">> EEPROM impl (raw) testing app");

  while(true)
  {
    auto const cmd = uart.rx();
    if(not cmd)
      continue;

    uint32_t const v = 0x12345678u;
    if( not eeprom.write(0, v) )
    {
      write_line_fmt(uart, "FAILED to write test value: 0x%x", v);
      continue;
    }
    write_line_fmt(uart, "wrote test value: 0x%x", v);

    auto const r = eeprom.read(0);
    if(not r)
    {
      write_line(uart, "FAILED to read test value");
      continue;
    }
    write_line_fmt(uart, "read test value: 0x%x", *r);
  }
}
