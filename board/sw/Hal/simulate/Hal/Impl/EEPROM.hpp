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
      case 1: sim().min_position_ = value / u32_max_f; return true;
      case 2: sim().max_position_ = value / u32_max_f; return true;
      case 3: sim().LED_brightness_ = value / u32_max_f; return true;
    }
    throw std::runtime_error{"EEPROM::write(): unknown slot: " + std::to_string(slot)};
  }

  std::optional<uint32_t> read(size_t slot) const
  {
    switch(slot)
    {
      case 0: return sim().marker_;
      case 1: return round( sim().min_position_ * u32_max_i );
      case 2: return round( sim().max_position_ * u32_max_i );
      case 3: return round( sim().LED_brightness_ * u32_max_i );
    }
    throw std::runtime_error{"EEPROM::read(): unknown slot: " + std::to_string(slot)};
  }

private:
  static constexpr auto u32_max_i = std::numeric_limits<uint32_t>::max();
  static constexpr auto u32_max_f = static_cast<float>(u32_max_i);
};

}
