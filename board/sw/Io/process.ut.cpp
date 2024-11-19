#include "catch2/catch.hpp"
#include "Io/process.hpp"

using Io::Line;
using Io::process;
using namespace Io::Proto;

namespace
{

struct Handler
{
  Get_persistent_config::Reply handle(Get_persistent_config::Request const& req)
  {
    return {
      .min_pos_ = 42,
      .max_pos_ = 142,
      .LED_brightness_ = 66
    };
  }
  Get_servo_position::Reply handle(Get_servo_position::Request const& req)
  {
    return {};    // TODO
  }
  Get_telemetry::Reply handle(Get_telemetry::Request const& req)
  {
    return {};    // TODO
  }
  Ping::Reply handle(Ping::Request const& req)
  {
    return {};    // TODO
  }
  Set_LED_brightness::Reply handle(Set_LED_brightness::Request const& req)
  {
    return {};    // TODO
  }
  Set_max_servo_position::Reply handle(Set_max_servo_position::Request const& req)
  {
    return {};    // TODO
  }
  Set_min_servo_position::Reply handle(Set_min_servo_position::Request const& req)
  {
    return {};    // TODO
  }
  Set_servo_position::Reply handle(Set_servo_position::Request const& req)
  {
    return {};    // TODO
  }
};


auto mk_cmd(std::string_view str)
{
  Line line;
  for(auto i=0u; i<str.size(); ++i)
    line.data_[i] = str[i];
  add_checksum(line);
  return line;
}

TEST_CASE("test basic lines")
{
  (void)mk_cmd;
}


}
