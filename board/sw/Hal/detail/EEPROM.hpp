#pragma once
#include <cinttypes>

namespace Hal::detail::EEPROM
{

static constexpr uint8_t marker_set_value{0x42};

static constexpr size_t address_marker{  0 };
static constexpr size_t address_min_pos{ address_marker  + sizeof(uint8_t) };
static constexpr size_t address_max_pos{ address_min_pos + sizeof(float) };
static constexpr size_t address_LED{     address_max_pos + sizeof(float) };

}
