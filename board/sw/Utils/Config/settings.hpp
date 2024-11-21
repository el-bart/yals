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
constexpr auto servo_traven_exclusion_zone_mm = (potentiometer_precision_percent / 100.0f) * servo_traven_len_mm;

// max theoretical travel distance of a servo (i.e. potentiometer length)
constexpr auto servo_absolute_min = servo_traven_exclusion_zone_mm;
constexpr auto servo_absolute_max = servo_traven_len_mm - servo_traven_exclusion_zone_mm;

// 0..1 tolerance value. 0.5 factor represents that an actual value should be within +/-tolerance of a center point.
constexpr auto servo_position_tolerance = 0.5f * servo_position_tolerance_mm / servo_traven_len_mm;
}
