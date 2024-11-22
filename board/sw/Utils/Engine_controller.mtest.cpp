#include "Utils/Engine_controller.hpp"
#include "Hal/Position_feedback.hpp"
#include "Utils/write_helpers.hpp"

using Utils::write;
using Utils::write_line;
using Utils::write_line_fmt;


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

  while(true)
  {
    for(auto preset: {0.1, 0.9})
    {
      while(true)
      {
        auto const p = pos.value();
      }
      sleep_ms(500);
    }
  }
}
