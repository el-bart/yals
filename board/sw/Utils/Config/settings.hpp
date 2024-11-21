// extends "config.hpp" with other, pre-computed values.
// aimed to be used in the code.
#pragma once
#include "Utils/Config/config.hpp"

namespace Utils::Config
{
// configures how much on min / max position is not available, to compensate for non-perfect potentiometer.
// note that this does not change min / max and position values returned. it is only applied to limit the
// actual movement of the carriage (i.e. off-limit of 5mm with position set to 0, will stop at 5mm and
// report 5mm as a current position ad infinitum, even though min pos is set to 0).
constexpr auto servo_traven_exclusion_zone_mm = (potentiometer_precision_percent / 100.0f) * servo_traven_len_mm; // mm

// max theoretical travel distance of a servo (i.e. potentiometer length), cut but the potentiometer
// precision limits. this helps to ensure wall will not be hit.
constexpr auto servo_absolute_min_mm = servo_traven_exclusion_zone_mm;
constexpr auto servo_absolute_max_mm = servo_traven_len_mm - servo_traven_exclusion_zone_mm;
// same as above, but in 0..1 range.
constexpr auto servo_absolute_min = servo_absolute_min_mm / servo_traven_len_mm;
constexpr auto servo_absolute_max = servo_absolute_max_mm / servo_traven_len_mm;

// 0..1 value for representing servo position tolerance (i.e. unit-agnostic value).
constexpr auto servo_position_tolerance = servo_position_tolerance_mm / servo_traven_len_mm;
// 0..1 value representing servo position histeresis (i.e. unit-agnostic value).
constexpr auto servo_position_histeresis = servo_position_histeresis_mm / servo_traven_len_mm;

// frequency represented as a loop time (it's more convenient to use in the code)
constexpr auto control_loop_time = 1.0f / control_loop_frequency; // 1/f
}
