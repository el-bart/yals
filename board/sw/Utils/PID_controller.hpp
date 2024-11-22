#pragma once
#include "Hal/Engine.hpp"
#include "Hal/Clock.hpp"
#include "Utils/Config/settings.hpp"
#include <algorithm>
#include <cmath>

namespace Utils
{

struct PID_controller final
{
  PID_controller() = default;

  PID_controller(PID_controller const&) = delete;
  PID_controller& operator=(PID_controller const&) = delete;
  PID_controller(PID_controller&&) = delete;
  PID_controller& operator=(PID_controller&&) = delete;

  // returns -1..+1 value reprenseting:
  // * 0..1 of engine throttle
  // * direction with a sign (negative is left, positive is right)
  float update(double const in_dt_s, double const preset_position, double const current_position)
  {
    auto const error = preset_position - current_position;

    using namespace ::Utils::Config;
    auto const dt_s = std::max(control_loop_time_s / 10.0, in_dt_s);   // in_dt_s may be ~0 in 1st iteration after system start
    // P
    auto const p = PID_controller_P * error;
    // I
    integral_ += error * dt_s;
    auto const i = PID_controller_I * integral_;
    // D
    auto const derivative = (error - prev_error_) / dt_s;
    auto const d = PID_controller_D * derivative;

    prev_error_ = error;

    auto const pid = p + i + d;
    return std::clamp(pid, -1.0, +1.0);
  }

private:
  double prev_error_{};
  double integral_{};
};

}
