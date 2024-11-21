#include <catch2/catch.hpp>
#include "Handler.hpp"
#include "Hal/Sim.hpp"

using Hal::sim;
using namespace Utils::Config;

namespace
{

TEST_CASE("Handler")
{
  sim().reset();
  Context ctx;
  Handler h{ctx};

  SECTION("setting servo position")
  {
    using namespace Io::Proto::Set_servo_position;
    ctx.setpoints_.position_ = 500.0/999.0;

    SECTION("using correct value")
    {
      auto const r = h.handle( Request{ .pos_ = 123 } );
      REQUIRE( r.err_ == nullptr );
      CHECK( ctx.setpoints_.position_ == Approx(123.0/999.0) );
    }

    SECTION("obeying SW limits")
    {
      SECTION("below min value")
      {
        ctx.setpoints_.min_pos_ = 200.0/999.0;
        auto const r = h.handle( Request{ .pos_ = 199 } );
        REQUIRE( r.err_ == std::string{"below min"} );
        CHECK( ctx.setpoints_.position_ == Approx(500.0/999.0) );
      }

      SECTION("above maxvalue")
      {
        ctx.setpoints_.max_pos_ = 900.0/999.0;
        auto const r = h.handle( Request{ .pos_ = 901 } );
        REQUIRE( r.err_ == std::string{"above max"} );
        CHECK( ctx.setpoints_.position_ == Approx(500.0/999.0) );
      }
    }

    SECTION("obeying absolute limits")
    {
      SECTION("below abs min value")
      {
        if(servo_absolute_min == 0.0)
          return;
        auto const r = h.handle( Request{ .pos_ = 0 } );
        REQUIRE( r.err_ != nullptr );
        CHECK( r.err_ == std::string{"below abs min"} );
        CHECK( ctx.setpoints_.position_ == Approx(500.0/999.0) );
      }

      SECTION("above abs max value")
      {
        if(servo_absolute_max == 1.0)
          return;
        auto const r = h.handle( Request{ .pos_ = 999 } );
        REQUIRE( r.err_ != nullptr );
        CHECK( r.err_ == std::string{"above abs max"} );
        CHECK( ctx.setpoints_.position_ == Approx(500.0/999.0) );
      }
    }
  }
}

}
