#include "catch2/catch.hpp"
#include "Hal/all.hpp"

namespace
{

TEST_CASE("Hal::sim() is global")
{
  Hal::sim().amps_ = 13;
  REQUIRE(Hal::sim().amps_ == 13);
}

}
