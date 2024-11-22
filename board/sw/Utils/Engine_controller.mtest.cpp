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
  Hal::sleep( std::chrono::microseconds{ static_cast<uint64_t>(Utils::Config::control_loop_time * 1'000'000) } );
}

void go_to_position(Utils::Engine_controller& ec, Hal::Position_feedback& pos, float const preset)
{
  float p;
  do
  {
    p = pos.value();
    ec.update(preset, p);
    ctrl_loop_cycle_pause();
  }
  while( fabs(p - preset) > Utils::Config::servo_position_tolerance_mm );
}

int main()
{
  Hal::Uart uart;
  Hal::Position_feedback pos;
  Hal::Engine eng;
  Hal::Clock clock;
  Utils::Engine_controller ec{eng, clock, pos.value()};

  write_line(uart, ">>");
  write_line(uart, ">> engine controller - jogging app");
  write_line(uart, ">>");

  Utils::purge_rx(uart);

#if 0
  for(auto n=0u;; ++n)
#else
  for(auto n=0u; n<3u; ++n)
#endif
    for(auto preset: {0.2, 0.8})
    {
      write_line_fmt(uart, ">> #%u moving to pos=%f", n, preset);
      go_to_position(ec, pos, preset);

      write_line(uart, ">> short pause between cycles");
      auto const deadline = clock.now() + clock.ticks_per_second();
      while( clock.now() < deadline )
        go_to_position(ec, pos, preset);
      write_line(uart, "");
    }

  write_line(uart, ">> done! parking in the middle...");
  while(true)
    go_to_position(ec, pos, 0.5);
}
