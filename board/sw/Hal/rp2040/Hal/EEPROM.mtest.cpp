#include "Hal/EEPROM.hpp"
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

void print_help(Hal::Uart& uart)
{
  for(auto str: {
      ">> help:",
      "h - help",
      "a - min++",
      "z - min--",
      "d - max++",
      "c - max--",
      "r - read all from EEPROM",
      "w - write EEPROM-set mark",
      "x - write EEPROM defaults and mark as set",
  })
    write_line(uart, str);
}


void read_all(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const min = eeprom.min_position();
  auto const max = eeprom.max_position();
  auto const mark_set = eeprom.marker_check();
  write_line_fmt(uart, ">> min=%f max=%f mark:%s", min, max, mark_set ? "set" : "unset");
}

void write_mark(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  eeprom.marker_write();
  write_line(uart, ">> EEPROM-set marker written");
}


void write_defaults(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  eeprom.min_position(0.0);
  eeprom.max_position(1.0);
  eeprom.marker_write();
  write_line(uart, ">> defaults written");
}


void increment_min(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev  = eeprom.min_position();
  auto const next = std::clamp(prev + 0.1f, 0.0f, 1.0f);
  eeprom.min_position(next);
  write_line_fmt(uart, ">> min++ from %f to %f", prev, next);
  read_all(uart, eeprom);
}

void decrement_min(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev  = eeprom.min_position();
  auto const next = std::clamp(prev - 0.1f, 0.0f, 1.0f);
  eeprom.min_position(next);
  write_line_fmt(uart, ">> min-- from %f to %f", prev, next);
  read_all(uart, eeprom);
}

void increment_max(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev  = eeprom.max_position();
  auto const next = std::clamp(prev + 0.1f, 0.0f, 1.0f);
  eeprom.max_position(next);
  write_line_fmt(uart, ">> max++ from %f to %f", prev, next);
  read_all(uart, eeprom);
}

void decrement_max(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev  = eeprom.max_position();
  auto const next = std::clamp(prev - 0.1f, 0.0f, 1.0f);
  eeprom.max_position(next);
  write_line_fmt(uart, ">> max-- from %f to %f", prev, next);
  read_all(uart, eeprom);
}


int main()
{
  Hal::EEPROM eeprom;
  Hal::Uart uart;

  write_line(uart, ">> EEPROM testing app");

  while(true)
  {
    auto const cmd = uart.rx();
    if(not cmd)
      continue;

    switch(*cmd)
    {
      case 'a': increment_min(uart, eeprom); break;
      case 'z': decrement_min(uart, eeprom); break;

      case 'd': increment_max(uart, eeprom); break;
      case 'c': decrement_max(uart, eeprom); break;

      case 'r': read_all(uart, eeprom); break;
      case 'w': write_mark(uart, eeprom); break;
      case 'x': write_defaults(uart, eeprom); break;

      case 'h':
      default:
        print_help(uart); break;
    }
  }
}
