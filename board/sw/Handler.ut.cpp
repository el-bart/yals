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
        ctx.setpoints_.max_pos_ = 800.0/999.0;
        auto const r = h.handle( Request{ .pos_ = 801 } );
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


  SECTION("setting min servo position")
  {
    using namespace Io::Proto::Set_min_servo_position;
    ctx.setpoints_.position_ = 500.0/999.0;
    ctx.setpoints_.min_pos_  = 200.0/999.0;
    ctx.setpoints_.max_pos_  = 800.0/999.0;

    SECTION("using correct value")
    {
      auto const r = h.handle( Request{ .min_pos_ = 123 } );
      REQUIRE( r.err_ == nullptr );
      CHECK( ctx.setpoints_.min_pos_ == Approx(123.0/999.0) );
    }

    SECTION("obeying absolute limits: min value")
    {
      if(servo_absolute_min == 0.0)
        return;
      auto const r = h.handle( Request{ .min_pos_ = 0 } );
      REQUIRE( r.err_ != nullptr );
      CHECK( r.err_ == std::string{"below abs min"} );
      CHECK( ctx.setpoints_.position_ == Approx(500.0/999.0) );
      CHECK( ctx.setpoints_.min_pos_  == Approx(200.0/999.0) );
      CHECK( ctx.setpoints_.max_pos_  == Approx(800.0/999.0) );
    }

    SECTION("obeying logic: min <= max")
    {
      auto const r = h.handle( Request{ .min_pos_ = 801 } );
      REQUIRE( r.err_ != nullptr );
      CHECK( r.err_ == std::string{"above max"} );
      CHECK( ctx.setpoints_.position_ == Approx(500.0/999.0) );
      CHECK( ctx.setpoints_.min_pos_  == Approx(200.0/999.0) );
      CHECK( ctx.setpoints_.max_pos_  == Approx(800.0/999.0) );
    }
  }


  SECTION("setting max servo position")
  {
    using namespace Io::Proto::Set_max_servo_position;
    ctx.setpoints_.position_ = 500.0/999.0;
    ctx.setpoints_.min_pos_  = 200.0/999.0;
    ctx.setpoints_.max_pos_  = 800.0/999.0;

    SECTION("using correct value")
    {
      auto const r = h.handle( Request{ .max_pos_ = 789 } );
      REQUIRE( r.err_ == nullptr );
      CHECK( ctx.setpoints_.max_pos_ == Approx(789.0/999.0) );
    }

    SECTION("obeying absolute limits: max value")
    {
      if(servo_absolute_max == 0.0)
        return;
      auto const r = h.handle( Request{ .max_pos_ = 999 } );
      REQUIRE( r.err_ != nullptr );
      CHECK( r.err_ == std::string{"above abs max"} );
      CHECK( ctx.setpoints_.position_ == Approx(500.0/999.0) );
      CHECK( ctx.setpoints_.min_pos_  == Approx(200.0/999.0) );
      CHECK( ctx.setpoints_.max_pos_  == Approx(800.0/999.0) );
    }

    SECTION("obeying logic: min <= max")
    {
      auto const r = h.handle( Request{ .max_pos_ = 199 } );
      REQUIRE( r.err_ != nullptr );
      CHECK( r.err_ == std::string{"below min"} );
      CHECK( ctx.setpoints_.position_ == Approx(500.0/999.0) );
      CHECK( ctx.setpoints_.min_pos_  == Approx(200.0/999.0) );
      CHECK( ctx.setpoints_.max_pos_  == Approx(800.0/999.0) );
    }
  }
}

}
