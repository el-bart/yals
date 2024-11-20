#pragma once
#include "Hal/Sim.hpp"
#include <stdexcept>
#include <optional>
#include <cmath>

namespace Hal::Impl
{

struct EEPROM
{
  EEPROM() = default;

  EEPROM(EEPROM const&) = delete;
  EEPROM& operator=(EEPROM const&) = delete;
  EEPROM(EEPROM &&) = delete;
  EEPROM& operator=(EEPROM &&) = delete;

  bool write(size_t slot, uint32_t value)
  {
    switch(slot)
    {
      case 0: sim().marker_ = value; return true;
      case 1: sim().min_position_ = value / u32_max_d; return true;
      case 2: sim().max_position_ = value / u32_max_d; return true;
      case 3: sim().EEPROM_LED_brightness_ = value / u32_max_d; return true;
    }
    throw std::runtime_error{"EEPROM::write(): unknown slot: " + std::to_string(slot)};
  }

  std::optional<uint32_t> read(size_t slot) const
  {
    switch(slot)
    {
      case 0: return sim().marker_;
      case 1: return float_to_uint32( sim().min_position_ );
      case 2: return float_to_uint32( sim().max_position_ );
      case 3: return float_to_uint32( sim().EEPROM_LED_brightness_ );
    }
    throw std::runtime_error{"EEPROM::read(): unknown slot: " + std::to_string(slot)};
  }

private:
  static constexpr auto u32_max_i = std::numeric_limits<uint32_t>::max();
  static constexpr auto u32_max_d = static_cast<double>(u32_max_i); // need doubles here to avoid numerical fluxes...

  static uint32_t float_to_uint32(float v)
  {
    return std::clamp( round( v * u32_max_d ), 0.0, u32_max_d );
  }
};

}
