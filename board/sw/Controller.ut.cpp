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
    auto const& ctx = ctrl.context();
    CHECK( ctx.setpoints_.position_ == Approx( sim().position_ * 100.0 ) );
    // TODO... unless it's outside of min / max range...
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
    CHECK( ctx.last_reads_.position_percent_ == sim().position_ * 100 );
  }
}

}
