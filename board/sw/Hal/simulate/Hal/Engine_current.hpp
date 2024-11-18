#pragma once
#include "Hal/Sim.hpp"

namespace Hal
{

struct Engine_current
{
  Engine_current() = default;

  Engine_current(Engine_current const&) = delete;
  Engine_current& operator=(Engine_current const&) = delete;
  Engine_current(Engine_current &&) = delete;
  Engine_current& operator=(Engine_current &&) = delete;

  float amps()
  {
    sim().update();
    return sim().amps_;
  }
};

}
