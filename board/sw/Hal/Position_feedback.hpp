#pragma once
#include "Hal/Impl/Position_feedback.hpp"

namespace Hal
{

struct Position_feedback
{
  Position_feedback() = default;

  float percent()
  {
    return impl_.percent();
  }

private:
  Impl::Position_feedback impl_;
};

}
