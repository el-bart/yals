// contains default settings. aimed for user to control.
#pragma once

namespace Utils::Config
{
// max theoretical travel distance of a servo (i.e. potentiometer length)
constexpr auto servo_traven_len_mm = 100.0f; // mm

// linear potentiometer precision
constexpr auto potentiometer_precision_percent = 5u; // %

// default LED brightness (0..1)
constexpr auto default_LED_brightness = 0.3f;

// tolerance distance for servo (i.e. "how close is close enough not to bother?")
constexpr auto servo_position_tolerance_mm = 1.5f; // mm

// full-throttle distance of the motor, when difference between a preset position and
// actual position is at least equal to the above value.
constexpr auto engine_full_throttle_at_diff_mm = 5.0f; // mm

// engine min. force / torque that can be applied. values smaller than this will be clippe to it.
// exception is 0.0, that is interpreted as a literal 0.0 (no force).
// it ranges 0..1 here, where so 0.25 is 25% of force.
constexpr auto engine_min_force = 0.80f;

// expected frequency of Controller::update() operations.
constexpr auto control_loop_frequency = 250u; // Hz
}
