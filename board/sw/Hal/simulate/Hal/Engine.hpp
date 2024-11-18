#pragma once
#include "Hal/Sim.hpp"

namespace Hal
{

struct Engine
{
  enum Direction
  {
    Off,
    Left,
    Right
  };

  Engine() = default;

  Engine(Engine const&) = delete;
  Engine& operator=(Engine const&) = delete;
  Engine(Engine &&) = delete;
  Engine& operator=(Engine &&) = delete;

  void direction(const Direction dir)
  {
    dir_ = dir;
    apply();
  }

  void force(const uint16_t f)
  {
    force_ = f;
    apply();
  }

  void set(const Direction dir, const uint16_t f)
  {
    dir_ = dir;
    force_ = f;
    apply();
  }

private:
  void apply() const
  {
    switch(dir_)
    {
      case Direction::Off:    sim().engine_force_ = 0;        break;
      case Direction::Left:   sim().engine_force_ = +force_;  break;
      case Direction::Right:  sim().engine_force_ = -force_;  break;
    }
  }

  Direction dir_{Direction::Off};
  uint16_t force_{0};
};

}
