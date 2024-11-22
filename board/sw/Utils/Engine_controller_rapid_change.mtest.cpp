#include "Utils/Engine_controller.hpp"
#include "Hal/Position_feedback.hpp"
#include "Hal/sleep.hpp"
#include "Utils/write_helpers.hpp"
#include "Utils/Config/settings.hpp"

using Utils::write;
using Utils::write_line;
using Utils::write_line_fmt;

void ctrl_loop_cycle_pause()
{
  // keep ~constant processing pace
  Hal::sleep( std::chrono::microseconds{ static_cast<uint64_t>(Utils::Config::control_loop_time_s * 1'000'000) } );
}

void quit(Hal::Uart& uart, Utils::Engine_controller& ec, Hal::Position_feedback& pos)
{
  write_line(uart, ">> QUIT REQUESTED - PARKING IN THE MIDDLE...");
  while(true)
  {
    auto const p = pos.value();
    ec.update(0.5, p);
    ctrl_loop_cycle_pause();
  }
}

void go_to_position(Hal::Uart& uart, Utils::Engine_controller& ec, Hal::Position_feedback& pos, float const preset)
{
  float p;
  do
  {
    if(auto const c = uart.rx(); c && *c == 'q')
      quit(uart, ec, pos);
    p = pos.value();
    ec.update(preset, p);
    ctrl_loop_cycle_pause();
  }
  while( fabs(p - preset) > Utils::Config::servo_position_tolerance );
}

int main()
{
  Hal::Uart uart;
  Hal::Position_feedback pos;
  Hal::Engine eng;
  Hal::Clock clock;
  Utils::Engine_controller ec{eng, clock, pos.value()};

  write_line(uart, "");
  write_line(uart, "");
  write_line(uart, ">>");
  write_line(uart, ">> engine controller - rapid change app");
  write_line(uart, ">>");

  Utils::purge_rx(uart);

  write_line(uart, ">> centering");
  go_to_position(uart, ec, pos, 0.5f);

  write_line(uart, ">> go!");
  for(auto n=0u; true; ++n)
  {
    write_line_fmt(uart, ">> iteration #%u", n);
    auto constexpr off = 0.4f;

    // move to 1 direction and then immediately change to the other
    for(auto dir: {-1.0f, +1.0f})
    {
      auto const preset = 0.5f + dir * off;
      write_line_fmt(uart, ">> new preset=%f", preset);
      float p;
      do
      {
        if(auto const c = uart.rx(); c && *c == 'q')
          quit(uart, ec, pos);
        p = pos.value();
        ec.update(preset, p);
        ctrl_loop_cycle_pause();
      }
      while( fabs(p - preset) > off / 2.0f );
    }

    write_line(uart, ">> iteration completed");
    write_line(uart, "");
  }
}
