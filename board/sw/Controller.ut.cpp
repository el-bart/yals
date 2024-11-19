#include <catch2/catch.hpp>
#include "Controller.hpp"
#include "Hal/Sim.hpp"

using Hal::sim;

namespace
{

TEST_CASE("Controller's c-tor")
{
  sim().reset();
  sim().update(0.0);

  SECTION("on start current servo position is assumed to be a setpoint")
  {
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().position_ * 100.0 ) );
  }

  SECTION("on start, if current servo setpoint is below min, it's clamped to it")
  {
    // TODO: need EEPROM sim to work
    sim().min_position_ = 0.7;
    sim().position_ = 0.5;
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().min_position_ * 100.0 ) );
  }

  SECTION("on start, if current servo setpoint is below min, it's clamped to it")
  {
    // TODO: need EEPROM sim to work
    sim().max_position_ = 0.7;
    sim().position_ = 0.9;
    Controller ctrl;
    CHECK( ctrl.context().setpoints_.position_ == Approx( sim().max_position_ * 100.0 ) );
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
    // TODO: not yet implemented
    ctrl.update();
    auto const& ctx = ctrl.context();
    CHECK( ctx.last_reads_.engine_current_A_ == sim().amps_ );
    CHECK( ctx.last_reads_.vcc_V_ == sim().vcc_ );
    CHECK( ctx.last_reads_.position_percent_ == sim().position_ * 100 );
  }
}

}
