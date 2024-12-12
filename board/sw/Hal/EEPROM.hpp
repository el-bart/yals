#pragma once
#include "Hal/Impl/EEPROM.hpp"
#include "Hal/detail/EEPROM.hpp"
#include <optional>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cinttypes>

namespace Hal
{

struct EEPROM
{
  EEPROM() = default;

  bool                 min_position(float const value) { return write_float(detail::EEPROM::address_min_pos, value); }
  std::optional<float> min_position() const            { return read_float(detail::EEPROM::address_min_pos);         }

  bool                 max_position(float const value) { return write_float(detail::EEPROM::address_max_pos, value); }
  std::optional<float> max_position() const            { return read_float(detail::EEPROM::address_max_pos);         }

  bool marker_write()
  {
    return impl_.write(detail::EEPROM::address_marker, &detail::EEPROM::marker_set_value, sizeof(uint8_t));
  }
  std::optional<bool> marker_check() const
  {
    uint8_t m;
    if( not impl_.read(detail::EEPROM::address_marker, &m, sizeof(uint8_t)) )
      return {};
    return m == detail::EEPROM::marker_set_value;
  }

  bool                 LED_brightness(float const value) { return write_float(detail::EEPROM::address_LED, value); }
  std::optional<float> LED_brightness() const            { return read_float(detail::EEPROM::address_LED);         }

private:
  bool write_float(size_t const address, float const value)
  {
    return impl_.write(address, &value, sizeof(value));
  }

  std::optional<float> read_float(size_t const address) const
  {
    float value;
    if( not impl_.read(address, &value, sizeof(value)) )
      return {};
    return value;
  }

  Impl::EEPROM impl_;
};

}
