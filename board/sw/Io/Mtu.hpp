#pragma once
#include "Io/Buffer.hpp"
#include "Io/const.hpp"
#include <optional>
#include <array>

namespace Io
{

struct Mtu
{
  static constexpr uint8_t max_size = max_mtu_size;
  uint8_t size_{0};
  std::array<uint8_t, max_size> data_;
};


namespace detail
{
inline std::optional<Mtu> to_mtu(Buffer& b, uint8_t eom_pos)
{
  if(b.data_[0] != '!')
    return {};
  return {};
}
}


inline std::optional<Mtu> extract_mtu(Buffer& b)
{
  b.consume_leading_garbage();
  if(b.size_ < 1+2+1) // minimal valid frame size
    return {};
  uint8_t eom_pos = 0;
  for(auto i=0u; i<b.size_; ++i, ++eom_pos)
    if(b.data_[i] == '\n')
    {
      auto const mtu = detail::to_mtu(b, eom_pos);
      b.trim_by(eom_pos);
      return mtu;
    }
  return {};
}

}
