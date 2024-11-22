#pragma once
#include "Hal/Engine.hpp"
#include "Hal/Clock.hpp"
#include "Utils/Config/settings.hpp"
#include <algorithm>
#include <cmath>

namespace Utils
{

struct Engine_controller final
{
  Engine_controller(Hal::Engine& eng, Hal::Clock const& clock, float current_position):
    eng_{eng},
    clock_{clock},
    last_run_at_{ clock.now() },
    last_position_{current_position},
    last_preset_{current_position}
  { }

  Engine_controller(Engine_controller const&) = delete;
  Engine_controller& operator=(Engine_controller const&) = delete;
  Engine_controller(Engine_controller&&) = delete;
  Engine_controller& operator=(Engine_controller&&) = delete;

  void update(float const preset_position, float const current_position)
  {
    using namespace Utils::Config;
    auto const now = clock_.now();
    update_impl(now, preset_position, current_position);
    // wrap up
    last_run_at_ = now;
    last_preset_ = preset_position;
    last_position_ = current_position;
  }

private:
  using Dir = Hal::Engine::Direction;

  void update_impl(Timepoint const now, float const preset_position, float const current_position)
  {
    using namespace Utils::Config;

    if( preset_position != last_preset_ )
      preset_reached_ = false;

    auto const dp = fabsf(preset_position - current_position);
    if( dp < servo_position_tolerance )
      return stop();
    if(preset_reached_)
      if( dp < servo_position_histeresis )
        return stop();
    move(now, preset_position, current_position);
  }

  inline void stop()
  {
    eng_.set(Dir::Off, 0);
    preset_reached_ = true;
  }

  inline void move(Timepoint const now, float preset_position, float current_position)
  {
    using namespace Utils::Config;
    auto const dest_pos_delta_mm = (preset_position  - current_position) * servo_traven_len_mm;    // how far are we from destination?
    //auto const last_pos_delta_mm = (current_position - last_position_  ) * servo_traven_len_mm;    // how far has ve travelled since last iteration?
    //auto const dt_s = (now - last_run_at_) / clock_.ticks_per_second();
    //auto const exp_d_pos_mmps = last_pos_delta_mm / dt_s;                                          // expected travel distance per update() in [mm/s]
    auto const force_coef = std::clamp( fabsf(dest_pos_delta_mm) / engine_full_throttle_at_diff_mm, engine_min_force, 1.0f );
    // compute final settings for engine
    auto const force = static_cast<uint32_t>( ceil(force_coef * 65535.0) );
    auto const dir = dest_pos_delta_mm < 0.0f ? Dir::Left : Dir::Right;
    eng_.set(dir, force);
  }

  Hal::Engine& eng_;
  Hal::Clock const& clock_;
  Timepoint last_run_at_;
  float last_position_{};
  float last_preset_{};
  bool preset_reached_{true};
};

}
