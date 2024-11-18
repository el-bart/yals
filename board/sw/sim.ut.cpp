#include "catch2/catch.hpp"
#include "Hal/all.hpp"

namespace
{

TEST_CASE("Hal::sim() is global")
{
  Hal::sim().amps_ = 13;
  REQUIRE(Hal::sim().amps_ == 13);
}

TEST_CASE("Hal::sim() can be reset")
{
  Hal::Sim s;
  s.engine_force_ = 13;
  s.amps_ = 0.42;
  s.led_brightness_ = 0.11;
  s.position_ = 0.69;
  s.rx_.push('r');
  s.tx_.push('t');
  s.vcc_ = 12.3;

  Hal::Sim const ref;
  s.reset();

  CHECK( s.engine_force_ == ref.engine_force_ );
  CHECK( s.amps_ == ref.amps_ );
  CHECK( s.led_brightness_ == ref.led_brightness_ );
  CHECK( s.position_ == ref.position_ );
  CHECK( s.rx_ == ref.rx_ );
  CHECK( s.tx_ == ref.tx_ );
  CHECK( s.vcc_ == ref.vcc_ );
}

}
