#pragma once
#include "Hal/Sim.hpp"
#include <stdexcept>
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

  void write(size_t slot, uint32_t value)
  {
    switch(slot)
    {
      case 0: sim().marker_ = value; return;
      case 1: sim().min_position_ = value / 65535.0f; return;
      case 2: sim().max_position_ = value / 65535.0f; return;
    }
    throw std::runtime_error{"EEPROM::write(): unknown slot: " + std::to_string(slot)};
  }

  uint32_t read(size_t slot) const
  {
    switch(slot)
    {
      case 0: return sim().marker_;
      case 1: return round( sim().min_position_ * 65535 );
      case 2: return round( sim().max_position_ * 65535 );
    }
    throw std::runtime_error{"EEPROM::read(): unknown slot: " + std::to_string(slot)};
  }
};

}
