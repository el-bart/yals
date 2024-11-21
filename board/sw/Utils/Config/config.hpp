// contains default settings. aimed for user to control.
#pragma once

namespace Utils::Config
{
// max theoretical travel distance of a servo (i.e. potentiometer length)
constexpr auto servo_traven_len_mm = 100.0f;

// configures how much on min / max position is not available, to compensate for non-perfect potentiometer
constexpr auto servo_traven_exclusion_zone_mm = 5.0f;

// default LED brightness (0..1)
constexpr auto default_LED_brightness = 0.3f;

// tolerance distance for servo (i.e. "how close is close enough not to bother?")
constexpr auto servo_position_tolerance_mm = 0.75f;

// full-throttle distance of the motor, when difference between a preset position and
// actual position is at least equal to the above value.
constexpr auto engine_full_throttle_at_diff_mm = 1.0f;
}
