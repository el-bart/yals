#pragma once
#include "Io/Line.hpp"
#include "Io/Proto/Get_persistent_config.hpp"
#include "Io/Proto/Get_servo_position.hpp"
#include "Io/Proto/Get_telemetry.hpp"
#include "Io/Proto/Ping.hpp"
#include "Io/Proto/Set_LED_brightness.hpp"
#include "Io/Proto/Set_max_servo_position.hpp"
#include "Io/Proto/Set_min_servo_position.hpp"
#include "Io/Proto/Set_servo_position.hpp"
#include <optional>

namespace Io
{

template<typename F>
std::optional<Line> process(Line const& line, F&& f)
{
  if(line.size_ < 3u) // definitely not valid
    return{};
  // TODO
  return {};
}

}
