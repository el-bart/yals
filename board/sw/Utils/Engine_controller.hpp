#pragma once
#include "Hal/Engine.hpp"
#include "Hal/Clock.hpp"

namespace Utils
{

struct Engine_controller final
{
  Engine_controller(Hal::Engine& eng, Hal::Clock const& clock, float current_position):
    eng_{eng},
    clock_{clock},
    last_run_at_{ clock.now() },
    last_position_{current_position}
  { }

  Engine_controller(Engine_controller const&) = delete;
  Engine_controller& operator=(Engine_controller const&) = delete;
  Engine_controller(Engine_controller&&) = delete;
  Engine_controller& operator=(Engine_controller&&) = delete;

  void update(float preset_position, float current_position)
  {
    //auto const now = clock_.now();
    //auto const dt_s = ( now - last_run_at_ ) / clock_.ticks_per_second();
    // TODO
  }

private:
  Hal::Engine& eng_;
  Hal::Clock const& clock_;
  Timepoint last_run_at_;
  float last_position_{};
};

}
