#include "catch2/catch.hpp"
#include "Utils/Engine_controller.hpp"
#include "Hal/Sim.hpp"

using Hal::sim;
using EC = Utils::Engine_controller;

namespace
{

TEST_CASE("Engine_controller")
{
  sim().reset();
  sim().position_ = 0.5;
  Hal::Engine eng;
  Hal::Clock const clock;
  EC ec{eng, clock, sim().position_};

  SECTION("on init no engine force is applied")
  {
    CHECK( sim().position_ == 0.5f );
    CHECK( sim().engine_force_ == 0u );
  }

  SECTION("when present matches current position, engine is stopped")
  {
    sim().update(0.5); // make sure some time passes
    ec.update(sim().position_, sim().position_);
    CHECK( sim().position_ == 0.5f );
    CHECK( sim().engine_force_ == 0u );
  }
}

}
