#pragma once
#include "Hal/Impl/EEPROM.hpp"
#include <optional>
#include <limits>
#include <cmath>
#include <cinttypes>

namespace Hal
{

struct EEPROM
{
  EEPROM() = default;

  bool                 min_position(float value) { return write_32(index_min_pos_, value); }
  std::optional<float> min_position() const      { return read_32(index_min_pos_);         }

  bool                 max_position(float value) { return write_32(index_max_pos_, value); }
  std::optional<float> max_position() const      { return read_32(index_max_pos_);         }

  bool                marker_write()       { return impl_.write(index_marker_, marker_); }
  std::optional<bool> marker_check() const
  {
    auto const m = impl_.read(index_marker_);
    if(not m)
      return {};
    return *m == marker_;
  }

  bool                 LED_brightness(float value) { return write_32(index_LED_, value); }
  std::optional<float> LED_brightness() const      { return read_32(index_LED_);         }

private:
  static constexpr auto u32_max = std::numeric_limits<uint32_t>::max();

  bool write_32(size_t slot, float value)
  {
    auto const n = static_cast<uint32_t>( round( value * u32_max ) );
    return impl_.write(slot, n);
  }

  std::optional<float> read_32(size_t slot) const
  {
    auto const n = impl_.read(slot);
    if(not n)
      return {};
    return *n / u32_max;
  }

  static constexpr uint32_t marker_{0x42};
  static constexpr size_t index_marker_{0};

  static constexpr size_t index_min_pos_{1};
  static constexpr size_t index_max_pos_{2};

  static constexpr size_t index_LED_{3};

  Impl::EEPROM impl_;
};

}
