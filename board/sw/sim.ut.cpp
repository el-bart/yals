#include "catch2/catch.hpp"
#include "Hal/All.hpp"

using Hal::Sim;
using Hal::sim;

namespace
{

TEST_CASE("Hal::sim() is global")
{
  sim().amps_ = 13;
  REQUIRE(Hal::sim().amps_ == 13);
}

TEST_CASE("Hal::Sim can be reset")
{
  Sim s;

  s.current_time_ =123456;
  s.engine_force_ = 13;
  s.amps_ = 0.42;
  s.vcc_ = 12.3;
  s.position_ = 0.69;
  s.LED_brightness_ = 0.11;

  s.rx_.push_back('r');
  s.tx_.push_back('t');

  s.marker_ = 0x123;
  s.min_position_ = 0.012345;
  s.max_position_ = 0.987654;
  s.EEPROM_LED_brightness_ = 0.45678;

  Sim const ref;
  s.reset();

  CHECK( s.current_time_ == ref.current_time_ );
  CHECK( s.engine_force_ == ref.engine_force_ );
  CHECK( s.amps_ == ref.amps_ );
  CHECK( s.vcc_ == ref.vcc_ );
  CHECK( s.position_ == ref.position_ );
  CHECK( s.LED_brightness_ == ref.LED_brightness_ );

  CHECK( s.rx_ == ref.rx_ );
  CHECK( s.tx_ == ref.tx_ );

  CHECK( s.marker_ == ref.marker_ );
  CHECK( s.min_position_ == ref.min_position_ );
  CHECK( s.max_position_ == ref.max_position_ );
  CHECK( s.EEPROM_LED_brightness_ == ref.EEPROM_LED_brightness_ );
}

SCENARIO("Hal::Sim properly integrates with Hal::All")
{
  sim().reset();
  Hal::All hal;
  auto const dt_short = Sim::eng_full_travel_time_s / 100.0f;
  auto const dt_long  = Sim::eng_full_travel_time_s *   2.0f;

  GIVEN("servo in the center position")
  {
    sim().position_ = 0.5;
    sim().update(dt_short);
    auto const idle_current = sim().amps_;

    WHEN("not doing anything")
    {
      sim().update(dt_short);
      sim().update(dt_long);
      THEN("position was not change")
      {
        CHECK( hal.pos_.value() == 0.5 );
      }
    }

    WHEN("applying force on motor to turn left")
    {
      hal.eng_.set(Hal::Engine::Direction::Left, 32'000);
      sim().update(dt_short);
      THEN("position has advanced")
      {
        CHECK( hal.pos_.value() > 0.0 );
        CHECK( hal.pos_.value() < 0.5 );
      }
      THEN("current has increased")
      {
        CHECK( sim().amps_ > idle_current );
      }
    }

    WHEN("applying force on motor to turn right")
    {
      hal.eng_.set(Hal::Engine::Direction::Right, 32'000);
      sim().update(dt_short);
      THEN("position has decremented")
      {
        CHECK( hal.pos_.value() > 0.5 );
        CHECK( hal.pos_.value() < 1.0 );
      }
      THEN("current has increased")
      {
        CHECK( sim().amps_ > idle_current );
      }
    }

    WHEN("applying force forward for a long time, top position is preserved")
    {
      hal.eng_.set(Hal::Engine::Direction::Left, 32'000);
      sim().update(dt_long);
      THEN("position has reached maximum")
      {
        CHECK( hal.pos_.value() == 0.0 );
      }
    }

    WHEN("applying force backward for a long time, top position is preserved")
    {
      hal.eng_.set(Hal::Engine::Direction::Right, 32'000);
      sim().update(dt_long);
      THEN("position has reached minimum")
      {
        CHECK( hal.pos_.value() == 1.0 );
      }
    }
  }

  GIVEN("uart data in RX buffers");
  {
    sim().rx_.push_back(0x13);
    sim().rx_.push_back(0x42);
    WHEN("receving data")
    {
      const auto b1 = hal.uart_.rx();
      const auto b2 = hal.uart_.rx();
      const auto b3 = hal.uart_.rx();
      THEN("1st byte is set")
      {
        REQUIRE(b1);
        CHECK( *b1 == 0x13 );
      }
      THEN("2nd byte is set")
      {
        REQUIRE(b2);
        CHECK( *b2 == 0x42 );
      }
      THEN("3rd byte is not set")
      {
        REQUIRE(not b3);
      }
    }
  }

  GIVEN("empty buffers")
  {
    WHEN("sending data")
    {
      hal.uart_.tx(0x13);
      hal.uart_.tx(0x42);
      THEN("data has been sent")
      {
        REQUIRE(sim().tx_.size() == 2u);
        REQUIRE(sim().tx_.front() == 0x13);
        sim().tx_.pop_front();
        REQUIRE(sim().tx_.front() == 0x42);
        sim().tx_.pop_front();
      }
    }
  }
}

}
