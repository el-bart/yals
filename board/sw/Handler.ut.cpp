#include <catch2/catch.hpp>
#include "Handler.hpp"

namespace
{

TEST_CASE("Handler's setting of servo position obeys absolute limits")
{
  Context ctx;
  Handler h{ctx};
}

}
