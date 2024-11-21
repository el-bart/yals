#pragma once
#include "Hal/Sim.hpp"

namespace Hal::Impl
{

struct Engine
{
  Engine() = default;

  Engine(Engine const&) = delete;
  Engine& operator=(Engine const&) = delete;
  Engine(Engine &&) = delete;
  Engine& operator=(Engine &&) = delete;

  void apply(int dir, uint16_t force) const
  {
    if(dir == 0)
    {
      sim().engine_force_ = 0;
      return;
    }
    if(dir > 0)
    {
      sim().engine_force_ = +force;
      return;
    }
    if(dir < 0)
    {
      sim().engine_force_ = -force;
      return;
    }
  }
};

}
