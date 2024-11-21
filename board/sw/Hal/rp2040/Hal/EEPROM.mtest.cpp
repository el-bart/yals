#include "Hal/EEPROM.hpp"
#include "Hal/Uart.hpp"
#include <algorithm>
#include <cstdio>
#include "Hal/Impl/write_helpers.hpp"

using Hal::Impl::write;
using Hal::Impl::write_line;
using Hal::Impl::write_line_fmt;

void print_help(Hal::Uart& uart)
{
  for(auto str: {
      ">> help:",
      "h - help",
      "a - min++",
      "z - min--",
      "d - max++",
      "c - max--",
      "g - brightness++",
      "b - brightness--",
      "r - read all from EEPROM",
      "w - write EEPROM-set mark",
      "x - write EEPROM defaults and mark as set",
      "",
  })
    write_line(uart, str);
}


void read_all(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const min = eeprom.min_position();
  auto const max = eeprom.max_position();
  auto const mark_set = eeprom.marker_check();
  auto const LED_brightness = eeprom.LED_brightness();
  if(not min || not max || not mark_set || not LED_brightness)
    write_line_fmt(uart, ">> EEPROM reading failed: %d %d %d %d", min.has_value(), max.has_value(), mark_set.has_value(), LED_brightness.has_value());
  else
    write_line_fmt(uart, ">> min=%f max=%f mark:%s LED_brightness=%f", *min, *max, *mark_set ? "set" : "unset", *LED_brightness);
}

void write_mark(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  if( eeprom.marker_write() )
    write_line(uart, ">> EEPROM-set marker written");
  else
    write_line(uart, ">> EEPROM-set marker writing failed");
}


void write_defaults(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const min = eeprom.min_position(0.0);
  auto const max = eeprom.max_position(1.0);
  auto const mark_set = eeprom.marker_write();
  if(not min || not max || not mark_set)
    write_line_fmt(uart, ">> EEPROM writing failed: %d %d %d", min, max, mark_set);
  else
    write_line(uart, ">> EEPROM defaults written");
  read_all(uart, eeprom);
}


void increment_min(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev = eeprom.min_position();
  if(not prev)
    return write_line(uart, ">> reading min pos failed");
  auto const next = std::clamp(*prev + 0.1f, 0.0f, 1.0f);
  if( not eeprom.min_position(next) )
    return write_line(uart, ">> writing min pos failed");
  write_line_fmt(uart, ">> min++ from %f to %f", *prev, next);
  read_all(uart, eeprom);
}

void decrement_min(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev = eeprom.min_position();
  if(not prev)
    return write_line(uart, ">> reading min pos failed");
  auto const next = std::clamp(*prev - 0.1f, 0.0f, 1.0f);
  if( not eeprom.min_position(next) )
    return write_line(uart, ">> writing min pos failed");
  write_line_fmt(uart, ">> min-- from %f to %f", prev, next);
  read_all(uart, eeprom);
}

void increment_max(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev = eeprom.max_position();
  if(not prev)
    return write_line(uart, ">> reading max pos failed");
  auto const next = std::clamp(*prev + 0.1f, 0.0f, 1.0f);
  if( not eeprom.max_position(next) )
    return write_line(uart, ">> writing max pos failed");
  write_line_fmt(uart, ">> max++ from %f to %f", prev, next);
  read_all(uart, eeprom);
}

void decrement_max(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev = eeprom.max_position();
  if(not prev)
    return write_line(uart, ">> reading max pos failed");
  auto const next = std::clamp(*prev - 0.1f, 0.0f, 1.0f);
  if( not eeprom.max_position(next) )
    return write_line(uart, ">> writing max pos failed");
  write_line_fmt(uart, ">> max-- from %f to %f", prev, next);
  read_all(uart, eeprom);
}

void increment_LED_brightness(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev = eeprom.LED_brightness();
  if(not prev)
    return write_line(uart, ">> reading LED brightness failed");
  auto const next = std::clamp(*prev + 0.1f, 0.0f, 1.0f);
  if( not eeprom.LED_brightness(next) )
    return write_line(uart, ">> writing LED brightness failed");
  write_line_fmt(uart, ">> LED brightness++ from %f to %f", prev, next);
  read_all(uart, eeprom);
}

void decrement_LED_brightness(Hal::Uart& uart, Hal::EEPROM& eeprom)
{
  auto const prev = eeprom.LED_brightness();
  if(not prev)
    return write_line(uart, ">> reading LED brightness failed");
  auto const next = std::clamp(*prev - 0.1f, 0.0f, 1.0f);
  if( not eeprom.LED_brightness(next) )
    return write_line(uart, ">> writing LED brightness failed");
  write_line_fmt(uart, ">> LED brightness-- from %f to %f", prev, next);
  read_all(uart, eeprom);
}


int main()
{
  Hal::EEPROM eeprom;
  Hal::Uart uart;

  write_line(uart, "");
  write_line(uart, "");
  write_line(uart, ">> EEPROM testing app");
  print_help(uart);
  read_all(uart, eeprom);

  // purge input
  while(uart.rx())
  { }

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

      case 'g': increment_LED_brightness(uart, eeprom); break;
      case 'b': decrement_LED_brightness(uart, eeprom); break;

      case 'r': read_all(uart, eeprom); break;
      case 'w': write_mark(uart, eeprom); break;
      case 'x': write_defaults(uart, eeprom); break;

      case 'h':
      default:
        print_help(uart); break;
    }
  }
}
