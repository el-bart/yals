#include <catch2/catch.hpp>
#include "Controller.hpp"
#include "Hal/Sim.hpp"

using Hal::sim;

namespace
{

TEST_CASE("Controller")
{
  sim().reset();
  Controller ctrl;
}

}
