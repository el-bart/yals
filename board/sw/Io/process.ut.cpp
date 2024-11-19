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
    return {};    // TODO
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
  uint8_t checksum = 0x00;
  for(auto i=0u; i<str.size(); ++i)
  {
    line.data_[i] = str[i];
    checksum ^= str[i];
  }
  line.size_ = str.size() + 2;
}

TEST_CASE("test basic lines")
{

}


}
