#include <catch2/catch.hpp>
#include "Controller.hpp"
#include "Hal/Sim.hpp"

using Hal::sim;
using namespace Utils::Config;

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

struct Reader
{
  std::string read_reply()
  {
    // make a copy to clear TX buffer so that more I/O can happen
    for(auto b: sim().tx_)
      received_.push_back(b);
    sim().tx_.clear();

    auto const eol = std::find( received_.begin(), received_.end(), '\n' );
    if( eol == received_.end() )
      return {};
    auto str = std::string{ received_.begin(), eol };
    received_.erase( received_.begin(), eol + 1 );

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

  std::deque<uint8_t> received_;
};


TEST_CASE("Controller's c-tor")
{
  sim().reset();
  sim().update(0.0);

  SECTION("on start current servo position is a new setpoint (i.e. do not move after start)")
  {
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().position_ ) );
  }

  SECTION("on start, if marker is not set, min and max positions as well as LED brightness are written to EEPROM")
  {
    sim().marker_ = 0xFFffFFff;     // i.e. unset
    sim().min_position_ = 0.4;
    sim().max_position_ = 0.6;
    sim().LED_brightness_ = 0.12;
    sim().position_ = 0.5;
    Controller ctrl;
    CHECK( sim().min_position_ == Approx(servo_absolute_min) );
    CHECK( sim().max_position_ == Approx(servo_absolute_max) );
    CHECK( sim().LED_brightness_ == Approx(Utils::Config::default_LED_brightness).epsilon(0.01) );
    CHECK( sim().marker_ == 0x42 );
    CHECK( ctrl.context().setpoints_.min_pos_ == Approx( sim().min_position_ ) );
    CHECK( ctrl.context().setpoints_.max_pos_ == Approx( sim().max_position_ ) );
    CHECK( ctrl.context().setpoints_.LED_brightness_ == Approx( sim().LED_brightness_ ).epsilon(0.01) );
  }

  SECTION("on start, if current servo setpoint is below min, it's clamped to it")
  {
    sim().min_position_ = 0.7;
    sim().position_ = 0.5;
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().min_position_ ) );
  }

  SECTION("on start, if current servo setpoint is above max, it's clamped to it")
  {
    sim().max_position_ = 0.7;
    sim().position_ = 0.9;
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().max_position_ ) );
  }

  SECTION("init clears all jung from RX buffer")
  {
    for(auto i=0; i<13; ++i)
      sim().rx_.push_back('a' + i);
    sim().rx_.push_back('\n');

    Reader reader;
    Controller ctrl;
    CHECK( sim().rx_.size() == 0u );
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "" );
  }
}


TEST_CASE("Controller")
{
  sim().reset();
  sim().update(0.0);
  sim().EEPROM_LED_brightness_ = 0.75;

  Reader reader;
  Controller ctrl;


  SECTION("LED is turned on by default")
  {
    CHECK( ctrl.context().setpoints_.LED_brightness_ == Approx(0.75f) );
    CHECK( sim().LED_brightness_ == Approx(0.75f).epsilon(0.01)  );
  }


  SECTION("update_and_apply() is non-lbocking")
  {
    ctrl.update_and_apply();
  }


  SECTION("update_and_apply() updates all reads")
  {
    ctrl.update_and_apply();
    auto const& ctx = ctrl.context();
    CHECK( ctx.last_reads_.engine_current_A_ == sim().amps_ );
    CHECK( ctx.last_reads_.vcc_V_ == sim().vcc_ );
    CHECK( ctx.last_reads_.position_ == sim().position_ );
  }


  SECTION("update_and_apply() handles Ping")
  {
    enqueue_command("~");
    ctrl.update_and_apply();
    auto reply = reader.read_reply();
    if(reply == "")
    {
      // depending on the exact version, reply string may be large here, thus may not fit into a single I/O turn
      ctrl.update_and_apply();
      reply = reader.read_reply();
    }
    CHECK( reply == std::string{"+"} + Utils::version_info()  );
  }


  SECTION("update_and_apply() handles Get_persistent_config")
  {
    enqueue_command("<090");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+" );
    enqueue_command(">890");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+" );
    enqueue_command("*81");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+" );

    REQUIRE( sim().min_position_   == Approx(  90.0f / 999.0f) );
    REQUIRE( sim().max_position_   == Approx( 890.0f / 999.0f) );
    REQUIRE( sim().LED_brightness_ == Approx(  81.0f /  99.0f).epsilon(0.05) );

    enqueue_command("?");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+<090>890*81" );
  }


  SECTION("update_and_apply() handles Get_servo_position")
  {
    sim().position_ = 0.42f;
    enqueue_command("!");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+420" );
  }


  SECTION("update_and_apply() handles Get_telemetry")
  {
    sim().amps_ = 1.25f;
    sim().vcc_  = 12.345f;
    enqueue_command("#");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+I1250U12345" );
  }


  SECTION("update_and_apply() handles Set_LED_brightness")
  {
    enqueue_command("*82");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+" );
    CHECK( sim().LED_brightness_ == Approx(0.82).epsilon(0.05)  );
    CHECK( sim().EEPROM_LED_brightness_ == Approx(0.82).epsilon(0.05)  );
  }


  SECTION("update_and_apply() handles Set_servo_position")
  {
    INFO( ctrl.context().setpoints_.min_pos_ );
    INFO( ctrl.context().setpoints_.max_pos_ );
    enqueue_command("@500");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+" );
    CHECK( ctrl.context().setpoints_.position_ == Approx(500.0/999.0) );
  }


  SECTION("update_and_apply() handles Set_max_servo_position")
  {
    enqueue_command(">900");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+" );
    CHECK( ctrl.context().setpoints_.max_pos_ == Approx(900.0/999.0) );
    CHECK( sim().max_position_                == Approx(900.0/999.0) );
  }


  SECTION("update_and_apply() handles Set_min_servo_position")
  {
    enqueue_command("<900");
    ctrl.update_and_apply();
    CHECK( reader.read_reply() == "+" );
    CHECK( ctrl.context().setpoints_.min_pos_ == Approx(900.0/999.0) );
    CHECK( sim().min_position_                == Approx(900.0/999.0) );
  }


  SECTION("update_and_apply() handles errors")
  {
    SECTION("on a known command")
    {
      auto const prev_pos = ctrl.context().setpoints_.position_;
      enqueue_command("@999");
      ctrl.update_and_apply();
      CHECK( reader.read_reply() == "-above abs max" );
      CHECK( ctrl.context().setpoints_.position_ == prev_pos );
    }

    SECTION("on an unknown command")
    {
      enqueue_command("abcdef");
      ctrl.update_and_apply();
      CHECK( reader.read_reply() == "-unknown cmd" );
    }

    SECTION("on a known command but with syntax error")
    {
      enqueue_command("@1234");
      ctrl.update_and_apply();
      CHECK( reader.read_reply() == "-cmd err" );
    }
  }


  SECTION("update_and_apply() handles engine control")
  {
    auto const dt = Hal::Sim::eng_full_travel_time_s / 100.0;
    sim().position_ = 500.0 / 999.0;

    SECTION("no movement if already there")
    {
      enqueue_command("@500");
      ctrl.update_and_apply();
      sim().update(dt);
      REQUIRE( reader.read_reply() == "+" );
      REQUIRE( ctrl.context().setpoints_.position_ == Approx(500.0/999.0) );
      CHECK( sim().position_ == Approx(500.0/999.0) );
      CHECK( sim().engine_force_ == 0u );
    }

    SECTION("move right")
    {
      enqueue_command("@600");
      ctrl.update_and_apply();
      sim().update(dt);
      REQUIRE( reader.read_reply() == "+" );
      REQUIRE( ctrl.context().setpoints_.position_ == Approx(600.0/999.0) );
      CHECK( sim().position_ > 500.0/999.0 );
      CHECK( sim().engine_force_ == 65535 );
    }

    SECTION("move left")
    {
      enqueue_command("@400");
      ctrl.update_and_apply();
      sim().update(dt);
      REQUIRE( reader.read_reply() == "+" );
      REQUIRE( ctrl.context().setpoints_.position_ == Approx(400.0/999.0) );
      CHECK( sim().position_ < 500.0/999.0 );
      CHECK( sim().engine_force_ == -65535 );
    }
  }


  SECTION("update_only() does I/O but does not apply")
  {
    auto const dt = Hal::Sim::eng_full_travel_time_s / 100.0;
    sim().position_ = 500.0 / 999.0;

    SECTION("move is not applied")
    {
      enqueue_command("@600");
      ctrl.update_only();
      sim().update(dt);
      REQUIRE( reader.read_reply() == "+" );
      REQUIRE( ctrl.context().setpoints_.position_ == Approx(600.0/999.0) );
      CHECK( sim().position_ == Approx(500.0/999.0) );
      CHECK( sim().engine_force_ == 0 );

      SECTION("next update_and_apply() does the job")
      {
        ctrl.update_and_apply();
        sim().update(dt);
        REQUIRE( reader.read_reply() == "" );
        CHECK( ctrl.context().setpoints_.position_ == Approx(600.0/999.0) );
        CHECK( sim().position_ > 500.0/999.0 );
        CHECK( sim().engine_force_ == 65535 );
      }
    }

    SECTION("LED brightness is not applued")
    {
      auto const prev_LED_brightness = sim().LED_brightness_;
      enqueue_command("*78");
      ctrl.update_only();
      sim().update(dt);
      REQUIRE( reader.read_reply() == "+" );
      CHECK( ctrl.context().setpoints_.LED_brightness_ == Approx(78.0/99.0).epsilon(0.05) );
      CHECK( sim().LED_brightness_ == prev_LED_brightness );

      SECTION("next update_and_apply() does the job")
      {
        ctrl.update_and_apply();
        sim().update(dt);
        REQUIRE( reader.read_reply() == "" );
        CHECK( ctrl.context().setpoints_.LED_brightness_ == Approx(78.0/99.0).epsilon(0.05) );
        CHECK( sim().LED_brightness_ == Approx(78.0/99.0).epsilon(0.05) );
      }
    }
  }
}

}
