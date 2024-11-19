#pragma once
#include "Io/Buffer.hpp"
#include "Io/const.hpp"
#include <optional>
#include <array>
#include <cassert>

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
inline std::optional<Mtu> to_mtu(Buffer const& b, uint8_t eom_pos)
{
  if(b.size_ <= eom_pos)
    return {};
  if(eom_pos < 2u)  // not even 1 byte - dropping
    return {};
  assert(b.data_[0] == '!');
  assert(b.data_[eom_pos] == '\n');
  Mtu mtu;
  std::copy(b.data_.begin() + 1u, b.data_.begin() + eom_pos, mtu.data_.begin());
  mtu.size_ = eom_pos - 1u;
  return mtu;
}
}


inline std::optional<Mtu> extract_mtu(Buffer& b)
{
  b.consume_leading_garbage();
  if(b.size_ < 2u)  // at least '!' and '\n' must be present here
    return {};
  for(auto i=1u; i<b.size_; ++i)
    if(b.data_[i] == '\n')
    {
      auto const mtu = detail::to_mtu(b, i);
      b.trim_by(i + 1u);
      return mtu;
    }
  return {};
}

}
