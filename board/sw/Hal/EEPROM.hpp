#pragma once
#include "Hal/Impl/EEPROM.hpp"
#include <optional>
#include <cmath>
#include <cinttypes>

namespace Hal
{

struct EEPROM
{
  EEPROM() = default;

  bool                 min_position(float value) { return write_pos(index_min_pos_, value); }
  std::optional<float> min_position() const      { return read_pos(index_min_pos_);  }

  bool                 max_position(float value) { return write_pos(index_max_pos_, value); }
  std::optional<float> max_position() const      { return read_pos(index_max_pos_);  }

  bool                marker_write()       { return impl_.write(index_marker_, marker_); }
  std::optional<bool> marker_check() const
  {
    auto const m = impl_.read(index_marker_);
    if(not m)
      return {};
    return *m == marker_;
  }

private:
  bool write_pos(size_t slot, float value)
  {
    auto const n = static_cast<uint32_t>( round( value * 65535 ) );
    return impl_.write(slot, n);
  }

  std::optional<float> read_pos(size_t slot) const
  {
    auto const n = impl_.read(slot);
    if(not n)
      return {};
    // make sure result is trimmed, as 32-bit value can go well above what is being technically asked for...
    auto const n16 = *n & 0xFFff;
    return n16 / 65535.0f;
  }

  static constexpr uint32_t marker_{0x42};
  static constexpr size_t index_marker_{0};

  static constexpr size_t index_min_pos_{1};
  static constexpr size_t index_max_pos_{2};

  Impl::EEPROM impl_;
};

}
