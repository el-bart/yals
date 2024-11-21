// extends "config.hpp" with other, pre-computed values.
// aimed to be used in the code.
#pragma once
#include "Utils/Config/config.hpp"

namespace Utils::Config
{
// max theoretical travel distance of a servo (i.e. potentiometer length)
constexpr auto servo_absolute_min = servo_traven_exclusion_zone_mm;
constexpr auto servo_absolute_max = servo_traven_len_mm - servo_traven_exclusion_zone_mm;
// 0..1 tolerance value. 0.5 factor represents, actual value should be within +/-tolerance of a center point.
constexpr auto servo_position_tolerance = 0.5f * servo_position_tolerance_mm / servo_traven_len_mm;
}
