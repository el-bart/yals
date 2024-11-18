#pragma once
#include "Hal/Sim.hpp"

namespace Hal
{

struct Position_feedback
{
  Position_feedback() = default;

  Position_feedback(Position_feedback const&) = delete;
  Position_feedback& operator=(Position_feedback const&) = delete;
  Position_feedback(Position_feedback &&) = delete;
  Position_feedback& operator=(Position_feedback &&) = delete;

  float percent()
  {
    return sim().position_ * 100.0f;
  }
};

}
