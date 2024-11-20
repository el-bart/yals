#include "catch2/catch.hpp"
#include "Io/process.hpp"
#include <map>

using Io::Line;
using Io::process;
using namespace Io::Proto;

namespace
{

struct Handler
{
  Get_persistent_config::Reply handle(Get_persistent_config::Request const& req)
  {
    calls_["Get_persistent_config"] += 1;
    return {
      .min_pos_ = 42,
      .max_pos_ = 142,
      .LED_brightness_ = 66
    };
  }

  Get_servo_position::Reply handle(Get_servo_position::Request const& req)
  {
    calls_["Get_servo_position"] += 1;
    return { .pos_{73} };
  }

  Get_telemetry::Reply handle(Get_telemetry::Request const& req)
  {
    calls_["Get_telemetry"] += 1;
    return {
      .eng_current_mA_ = 1234,
      .vcc_voltage_mV_ = 56789
    };
  }

  Ping::Reply handle(Ping::Request const& req)
  {
    calls_["Ping"] += 1;
    return { .pong_ = "YALS test" };
  }

  Set_LED_brightness::Reply handle(Set_LED_brightness::Request const& req)
  {
    calls_["Set_LED_brightness"] += 1;
    return {};
  }

  Set_max_servo_position::Reply handle(Set_max_servo_position::Request const& req)
  {
    calls_["Set_max_servo_position"] += 1;
    return {};
  }

  Set_min_servo_position::Reply handle(Set_min_servo_position::Request const& req)
  {
    calls_["Set_min_servo_position"] += 1;
    return {};
  }

  Set_servo_position::Reply handle(Set_servo_position::Request const& req)
  {
    calls_["Set_servo_position"] += 1;
    return {};
  }

  std::map<std::string, unsigned> calls_;
};


struct ErrorHandler
{
  Get_persistent_config::Reply handle(Get_persistent_config::Request const& req)
  { return { .err_ = "Eget_persistent_configE" }; }

  Get_servo_position::Reply handle(Get_servo_position::Request const& req)
  { return { .err_ = "Eget_servo_positionE" }; }

  Get_telemetry::Reply handle(Get_telemetry::Request const& req)
  { return { .err_ = "Eget_telemetryE" }; }

  Ping::Reply handle(Ping::Request const& req)
  { return { .err_ = "EpingE" }; }

  Set_LED_brightness::Reply handle(Set_LED_brightness::Request const& req)
  { return { .err_ = "Eset_led_brightnessE" }; }

  Set_max_servo_position::Reply handle(Set_max_servo_position::Request const& req)
  { return { .err_ = "Eset_max_servo_positionE" }; }

  Set_min_servo_position::Reply handle(Set_min_servo_position::Request const& req)
  { return { .err_ = "Eset_min_servo_positionE" }; }

  Set_servo_position::Reply handle(Set_servo_position::Request const& req)
  { return { .err_ = "Eset_servo_positionE" }; }
};


auto mk_cmd(std::string_view str, bool append_checksum = true)
{
  Line line;
  for(auto i=0u; i<str.size(); ++i)
    line.add_byte(str[i]);
  if(append_checksum)
    add_checksum(line);
  return line;
}

auto to_string(Line const& line, bool crop_checksum = true)
{
  auto str = std::string{ line.data_.data(), line.data_.data() + line.size_ };
  if( not checksum_valid(line) )
    throw std::runtime_error{"given line has invalid checksum: " + str};
  if(crop_checksum)
    str.resize( str.size() - 2u );
  return str;
}


template<typename H>
auto process_test(std::string_view cmd, H&& h)
{
  auto const c = mk_cmd(cmd);
  auto const l = process(c, h);
  return to_string(l);
}


TEST_CASE("process(): invalid line")
{
  Handler h;
  REQUIRE( h.calls_.empty() );

  SECTION("unkown command")
  {
    CHECK( process_test("xyz", h) == "-unknown cmd" );
    CHECK( h.calls_.empty() );
  }

  SECTION("valid command but checksum is missing")
  {
    CHECK( to_string( process( mk_cmd("@123", false), h ) ) == "-invalid checksum" );
    CHECK( h.calls_.empty() );
  }

  SECTION("valid command but checksum does not match")
  {
    auto line = mk_cmd("?");
    line.data_[line.size_ - 1u] = 'x';
    CHECK( to_string( process(line, h) ) == "-invalid checksum" );
    CHECK( h.calls_.empty() );
  }
}


TEST_CASE("process(): Get_persistent_config")
{
  SECTION("parsing with correct checksum")
  {
    Handler h;
    REQUIRE( h.calls_.empty() );
    CHECK( process_test("?", h) == "+<042>142*66" );
    CHECK( h.calls_.size() == 1 );
    CHECK( h.calls_["Get_persistent_config"] == 1 );
  }

  SECTION("parsing with error handler")
  {
    ErrorHandler h;
    CHECK( process_test("?", h) == "-Eget_persistent_configE" );
  }
}


TEST_CASE("process(): Get_servo_position")
{
  SECTION("parsing with correct checksum")
  {
    Handler h;
    REQUIRE( h.calls_.empty() );
    CHECK( process_test("!", h) == "+073" );
    CHECK( h.calls_.size() == 1 );
    CHECK( h.calls_["Get_servo_position"] == 1 );
  }

  SECTION("parsing with error handler")
  {
    ErrorHandler h;
    CHECK( process_test("!", h) == "-Eget_servo_positionE" );
  }
}


TEST_CASE("process(): Get_telemetry")
{
  SECTION("parsing with correct checksum")
  {
    Handler h;
    REQUIRE( h.calls_.empty() );
    CHECK( process_test("#", h) == "+I1234U56789" );
    CHECK( h.calls_.size() == 1 );
    CHECK( h.calls_["Get_telemetry"] == 1 );
  }

  SECTION("parsing with error handler")
  {
    ErrorHandler h;
    CHECK( process_test("#", h) == "-Eget_telemetryE" );
  }
}


TEST_CASE("process(): Ping")
{
  SECTION("parsing with correct checksum")
  {
    Handler h;
    REQUIRE( h.calls_.empty() );
    CHECK( process_test("~", h) == "+YALS test" );
    CHECK( h.calls_.size() == 1 );
    CHECK( h.calls_["Ping"] == 1 );
  }

  SECTION("parsing with error handler")
  {
    ErrorHandler h;
    CHECK( process_test("~", h) == "-EpingE" );
  }
}


TEST_CASE("process(): Set_LED_brightness")
{
  SECTION("parsing with correct checksum")
  {
    Handler h;
    REQUIRE( h.calls_.empty() );
    CHECK( process_test("*97", h) == "+" );
    CHECK( h.calls_.size() == 1 );
    CHECK( h.calls_["Set_LED_brightness"] == 1 );
  }

  SECTION("parsing with error handler")
  {
    ErrorHandler h;
    CHECK( process_test("*97", h) == "-Eset_led_brightnessE" );
  }
}


TEST_CASE("process(): Set_max_servo_position")
{
  SECTION("parsing with correct checksum")
  {
    Handler h;
    REQUIRE( h.calls_.empty() );
    CHECK( process_test(">997", h) == "+" );
    CHECK( h.calls_.size() == 1 );
    CHECK( h.calls_["Set_max_servo_position"] == 1 );
  }

  SECTION("parsing with error handler")
  {
    ErrorHandler h;
    CHECK( process_test(">997", h) == "-Eset_max_servo_positionE" );
  }
}


TEST_CASE("process(): Set_min_servo_position")
{
  SECTION("parsing with correct checksum")
  {
    Handler h;
    REQUIRE( h.calls_.empty() );
    CHECK( process_test("<321", h) == "+" );
    CHECK( h.calls_.size() == 1 );
    CHECK( h.calls_["Set_min_servo_position"] == 1 );
  }

  SECTION("parsing with error handler")
  {
    ErrorHandler h;
    CHECK( process_test("<321", h) == "-Eset_min_servo_positionE" );
  }
}


TEST_CASE("process(): Set_servo_position")
{
  SECTION("parsing with correct checksum")
  {
    Handler h;
    REQUIRE( h.calls_.empty() );
    CHECK( process_test("@678", h) == "+" );
    CHECK( h.calls_.size() == 1 );
    CHECK( h.calls_["Set_servo_position"] == 1 );
  }

  SECTION("parsing with error handler")
  {
    ErrorHandler h;
    CHECK( process_test("@678", h) == "-Eset_servo_positionE" );
  }
}

}
