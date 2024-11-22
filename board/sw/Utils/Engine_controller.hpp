#pragma once
#include "Hal/Engine.hpp"
#include "Hal/Clock.hpp"
#include "Utils/Config/settings.hpp"
#include "Utils/PID_controller.hpp"
#include <optional>
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
    {
      preset_reached_ = false;
      pid_.reset(); // prevents PID taking over when changing direction, while almost at the destination
    }

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
    pid_.reset();
  }

  inline void move(Timepoint const now, float preset_position, float current_position)
  {
    using namespace Utils::Config;
    auto force_coef = 0.0f;
    auto dir = Dir::Off;
    if(pid_)
    {
      auto const dt_s = (now - last_run_at_) / clock_.ticks_per_second();
      auto const pid = pid_->update(dt_s, preset_position, current_position);
      force_coef = fabsf(pid);
      dir = pid < 0.0f ? Dir::Left : Dir::Right;
    }
    else
    {
      auto const dest_pos_delta_mm = (preset_position  - current_position) * servo_traven_len_mm;    // how far are we from destination?
      force_coef = fabsf(dest_pos_delta_mm) / engine_full_throttle_at_diff_mm;
      dir = dest_pos_delta_mm < 0.0f ? Dir::Left : Dir::Right;
      // when approaching final position, enable PID
      if(not pid_ || force_coef < 1.0f)
        pid_.emplace();
    }
    // compute final settings for engine
    force_coef = std::clamp(force_coef, engine_min_force, 1.0f);
    auto const force = static_cast<uint32_t>( ceil(force_coef * 65535.0) );
    eng_.set(dir, force);
  }

  Hal::Engine& eng_;
  Hal::Clock const& clock_;
  Timepoint last_run_at_;
  float last_position_{};
  float last_preset_{};
  bool preset_reached_{true};
  std::optional<PID_controller> pid_;
};

}
