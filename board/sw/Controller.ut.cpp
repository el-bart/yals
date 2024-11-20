#include <catch2/catch.hpp>
#include "Controller.hpp"
#include "Hal/Sim.hpp"

using Hal::sim;

namespace
{

void enqueue_command(std::string_view cmd)
{
  // data
  for(auto c: cmd)
    sim().rx_.push_back(c);
  // no checksum
  sim().rx_.push_back('X');
  sim().rx_.push_back('X');
  // EOL
  sim().rx_.push_back('\n');
}

std::string read_reply()
{
  auto& buf = sim().tx_;
  auto const eol = std::find( buf.begin(), buf.end(), '\n' );
  if( eol == buf.end() )
    return {};
  auto str = std::string{ buf.begin(), eol };
  buf.erase( buf.begin(), eol + 1 );

  if( str.size() < 3u )
    throw std::runtime_error{"read_reply(): line too short: " + str};

  // remove checksum
  if( not isxdigit( str.back() ) )
    throw std::runtime_error{"read_reply(): invalid checksum chracter at the end (1): " + str};
  str.pop_back();
  if( not isxdigit( str.back() ) )
    throw std::runtime_error{"read_reply(): invalid checksum chracter at the end (2): " + str};
  str.pop_back();

  return str;
}


TEST_CASE("Controller's c-tor")
{
  sim().reset();
  sim().update(0.0);

  SECTION("on start current servo position is assumed to be a setpoint")
  {
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().position_ ) );
  }

  SECTION("on start, if marker is not set, min and max positions are set in EEPROM")
  {
    sim().marker_ = 0xFFffFFff;     // i.e. unset
    sim().min_position_ = 0.4;
    sim().max_position_ = 0.6;
    sim().position_ = 0.5;
    Controller ctrl;
    CHECK( sim().min_position_ == Approx(0.0) );
    CHECK( sim().max_position_ == Approx(1.0) );
    CHECK( sim().marker_ == 0x42 );
    CHECK( ctrl.context().setpoints_.min_pos_ == Approx( sim().min_position_ ) );
    CHECK( ctrl.context().setpoints_.max_pos_ == Approx( sim().max_position_ ) );
  }

  SECTION("on start, if current servo setpoint is below min, it's clamped to it")
  {
    sim().min_position_ = 0.7;
    sim().position_ = 0.5;
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().min_position_ ) );
  }

  SECTION("on start, if current servo setpoint is below min, it's clamped to it")
  {
    sim().max_position_ = 0.7;
    sim().position_ = 0.9;
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().max_position_ ) );
  }
}


TEST_CASE("Controller")
{
  sim().reset();
  sim().update(0.0);

  Controller ctrl;
  SECTION("update() is non-lbocking")
  {
    ctrl.update();
  }

  SECTION("update() updates all reads")
  {
    ctrl.update();
    auto const& ctx = ctrl.context();
    CHECK( ctx.last_reads_.engine_current_A_ == sim().amps_ );
    CHECK( ctx.last_reads_.vcc_V_ == sim().vcc_ );
    CHECK( ctx.last_reads_.position_ == sim().position_ );
  }

  SECTION("update() handles Ping")
  {
    enqueue_command("~");
    ctrl.update();
    CHECK( read_reply() == "+YALS" );
  }

  SECTION("update() handles Get_persistent_config")
  {
    return;         
    // TODO: these must be commands!
    sim().min_position_   =  90.0f / 999.0f;
    sim().max_position_   = 890.0f / 999.0f;
    sim().LED_brightness_ =  81.0f /  99.0f;
    enqueue_command("?");
    ctrl.update();
    CHECK( read_reply() == "+<090>890*81" );
  }

  SECTION("update() handles Get_servo_position")
  {
    sim().position_ = 0.42f;
    enqueue_command("!");
    ctrl.update();
    CHECK( read_reply() == "+420" );
  }

  SECTION("update() handles Get_telemetry")
  {
    sim().amps_ = 1.25f;
    sim().vcc_  = 12.345f;
    enqueue_command("#");
    ctrl.update();
    CHECK( read_reply() == "+I1250U12345" );
  }

  SECTION("update() handles Set_LED_brightness")
  {
    enqueue_command("*82");
    ctrl.update();
    CHECK( read_reply() == "+" );
    CHECK( sim().LED_brightness_ == Approx(0.82) );
  }
}

}
