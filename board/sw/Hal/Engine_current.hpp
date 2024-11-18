#pragma once
#include "Hal/Impl/Engine_current.hpp"

namespace Hal
{

struct Engine_current
{
  Engine_current() = default;

  float amps()
  {
    return impl_.amps();
  }

private:
  Impl::Engine_current impl_;
};

}
