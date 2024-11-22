// contains default settings. aimed for user to control.
#pragma once

namespace Utils::Config
{
// max theoretical travel distance of a servo (i.e. potentiometer length)
constexpr auto servo_traven_len_mm = 100.0f; // mm
// measured speed of travel of the servo
constexpr auto servo_travel_seed_mmps = 80.0f / 0.7f; // [mm/s]

// linear potentiometer precision
constexpr auto potentiometer_precision_percent = 5u; // %

// default LED brightness (0..1)
constexpr auto default_LED_brightness = 0.15f; // yes, it's very bright...

// tolerance distance for servo (i.e. "how close preset is close enough to stop adjusting?")
constexpr auto servo_position_tolerance_mm = 0.5f;  // mm
// histeresis of position (i.e. "how much readout must change before action is triggered?").
// note that this is applied only once preset position is reached (with servo_position_tolerance_mm
// precision). this value is ignored, when new preset position is set (even if new one is well
// within servo_position_histeresis_mm limit).
constexpr auto servo_position_histeresis_mm = 2.5f; // mm
static_assert( servo_position_tolerance_mm < servo_position_histeresis_mm );

// full-throttle distance of the motor, when difference between a preset position and
// actual position is at least equal to the above value.
constexpr auto engine_full_throttle_at_diff_mm = 5.0f; // mm

// engine min. force / torque that can be applied. values smaller than this will be clippe to it.
// exception is 0.0, that is interpreted as a literal 0.0 (no force).
// it ranges 0..1 here, where so 0.25 is 25% of force.
constexpr auto engine_min_force = 0.40f;

// expected frequency of engine control operations application.
// note that I/O still happens att full throttle.
constexpr auto control_loop_frequency = 250u; // Hz
}
