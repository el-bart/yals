#pragma once
#include "Hal/Sim.hpp"

namespace Hal::Impl
{

struct Position_feedback
{
  Position_feedback() = default;

  Position_feedback(Position_feedback const&) = delete;
  Position_feedback& operator=(Position_feedback const&) = delete;
  Position_feedback(Position_feedback &&) = delete;
  Position_feedback& operator=(Position_feedback &&) = delete;

  float value()
  {
    return sim().position_;
  }
};

}
