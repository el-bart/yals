#pragma once
#include "Hal/Impl/EEPROM.hpp"
#include <cmath>
#include <cinttypes>

namespace Hal
{

struct EEPROM
{
  EEPROM() = default;

  void  min_position(float value) { write_pos(index_min_pos_, value); }
  float min_position() const      { return read_pos(index_min_pos_);  }

  void  max_position(float value) { write_pos(index_max_pos_, value); }
  float max_position() const      { return read_pos(index_max_pos_);  }

  void marker_write()       { impl_.write(index_marker_, marker_); }
  bool marker_check() const { return impl_.read(index_marker_) == marker_; }

private:
  void write_pos(size_t slot, float value)
  {
    auto const n = static_cast<uint32_t>( round( value * 65535 ) );
    impl_.write(slot, n);
  }

  float read_pos(size_t slot) const
  {
    auto const n = impl_.read(slot);
    return n / 65535.0f;
  }

  static constexpr uint32_t marker_{0x42};
  static constexpr size_t index_marker_{0};

  static constexpr size_t index_min_pos_{1};
  static constexpr size_t index_max_pos_{2};

  Impl::EEPROM impl_;
};

}
