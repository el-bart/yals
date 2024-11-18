#pragma once
#include "Hal/Impl/Engine.hpp"

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
  void apply()
  {
    switch(dir_)
    {
      case Direction::Off:   return impl_.apply( 0,      0);
      case Direction::Left:  return impl_.apply(-1, force_);
      case Direction::Right: return impl_.apply(+1, force_);
    }
  }

  Direction dir_{Direction::Off};
  uint16_t force_{0};
  Impl::Engine impl_;
};

}
