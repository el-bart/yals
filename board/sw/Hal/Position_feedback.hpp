#pragma once
#include "Hal/Impl/Position_feedback.hpp"

namespace Hal
{

struct Position_feedback
{
  Position_feedback() = default;

  float value()
  {
    return impl_.value();
  }

private:
  Impl::Position_feedback impl_;
};

}
