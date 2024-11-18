#pragma once
#include "Hal/Sim.hpp"

namespace Hal::Impl
{

struct Vcc_feedback
{
  Vcc_feedback() = default;

  Vcc_feedback(Vcc_feedback const&) = delete;
  Vcc_feedback& operator=(Vcc_feedback const&) = delete;
  Vcc_feedback(Vcc_feedback &&) = delete;
  Vcc_feedback& operator=(Vcc_feedback &&) = delete;

  float volts()
  {
    return sim().vcc_;
  }
};

}
