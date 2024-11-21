#include "Hal/Engine.hpp"
#include "Hal/Uart.hpp"
#include "Hal/Position_feedback.hpp"
#include "Hal/Impl/write_helpers.hpp"
#include "Utils/Config/settings.hpp"

using Hal::Impl::write_line;
using Hal::Impl::write_line_fmt;
using E = Hal::Engine;

constexpr auto imp_long  = 0.1f;
constexpr auto imp_short = 0.003f;

void print_help(Hal::Uart& uart)
{
  write_line(uart, ">> help:");
  write_line(uart, "* h - help screen");
  write_line_fmt(uart, "* A - move forward  for %fs", imp_long);
  write_line_fmt(uart, "* a - move forward  for %fs", imp_short);
  write_line_fmt(uart, "* z - move backward for %fs", imp_short);
  write_line_fmt(uart, "* Z - move backward for %fs", imp_long);
  write_line(uart, "");
}

auto reverse(E::Direction dir)
{
  switch(dir)
  {
    case E::Direction::Right: return  E::Direction::Left;
    case E::Direction::Left:  return  E::Direction::Right;
    case E::Direction::Off:   return  E::Direction::Off;
  }
  assert(!"thu shall not pass!");
  return E::Direction::Off;
}

void move(Hal::Uart& uart, E& eng, Hal::Position_feedback& pos, E::Direction dir, float dt)
{
  write_line_fmt(uart, ">> moving into %s for %fs", dir == E::Direction::Left ? "left" : "right", dt);
  eng.set(dir, 65535);
  sleep_ms( dt * 1'000u );
#if 0
  // explicit brake
  write_line_fmt(uart, ">> brake");
  eng.set( reverse(dir) , 65535 );
  sleep_us(100);
#endif
  eng.direction(E::Direction::Off);
  auto const p = pos.value();
  auto const dist = p * Utils::Config::servo_traven_len_mm;
  write_line_fmt(uart, ">> stopped at %fmm (%f%%)", dist, p*100.0f);
}


int main()
{
  E eng;
  Hal::Uart uart;
  Hal::Position_feedback pos;

  write_line(uart, ">>");
  write_line(uart, ">> engine testing app");
  write_line(uart, ">>");
  print_help(uart);
  Hal::Impl::purge_rx(uart);

  while(true)
  {
    auto const cmd = uart.rx();
    if(not cmd)
      continue;

    switch(*cmd)
    {
      case 'A': move(uart, eng, pos, E::Direction::Right, imp_long ); break;
      case 'a': move(uart, eng, pos, E::Direction::Right, imp_short); break;
      case 'z': move(uart, eng, pos, E::Direction::Left,  imp_short); break;
      case 'Z': move(uart, eng, pos, E::Direction::Left,  imp_long ); break;
      case 'h':
      default: print_help(uart); break;
    }
  }
}
