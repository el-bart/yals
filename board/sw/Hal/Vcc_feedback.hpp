#pragma once
#include "Hal/Impl/Vcc_feedback.hpp"

namespace Hal
{

struct Vcc_feedback
{
  Vcc_feedback() = default;

  float volts()
  {
    return impl_.volts();
  }

private:
  Impl::Vcc_feedback impl_;
};

}
