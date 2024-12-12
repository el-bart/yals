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

  bool write(size_t const address, void const* value, size_t const size)
  {
    auto *p = static_cast<uint8_t const*>(value);
    for(auto i=0u; i<size; ++i)
      sim().EEPROM_[address + i] = *(p + i);
    return true;
  }

  bool read(size_t const address, void* value, size_t const size) const
  {
    auto *p = static_cast<uint8_t*>(value);
    for(auto i=0u; i<size; ++i)
      *(p + i) = sim().EEPROM_[address + i];
    return true;
  }

private:
};

}
