#include "Controller.hpp"

using Utils::Ticks;
using namespace Utils::Config;

namespace
{
auto ticks_per_cycle(Hal::Clock const& clock)
{
  auto const tpc = clock.ticks_per_second().value_ * control_loop_time;
  return Ticks{ .value_ = static_cast<uint64_t>(round(tpc)) };
}
}

int main()
{
  Controller ctrl;
  auto const& clock = ctrl.context().hal_.clock_;
  auto const tpc = ticks_per_cycle(clock);

  auto deadline = clock.now() + tpc;
  while(true)
    if( clock.now() < deadline )
      ctrl.update_only();           // do I/O often
    else
    {
      ctrl.update_and_apply();      // engine control loop is to be kept at ~constant pace
      deadline = deadline + tpc;
    }
}
