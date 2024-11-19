#pragma once
#include "Io/Line.hpp"
#include <optional>

namespace Io
{

namespace detail
{
struct HexChecksum
{
  uint8_t high_{'X'};
  uint8_t low_{'X'};
};

inline bool operator==(HexChecksum lhs, HexChecksum rhs)
{
  return lhs.high_ == rhs.high_ && lhs.low_ == rhs.low_;
}
inline bool operator!=(HexChecksum lhs, HexChecksum rhs)
{
  return not (lhs == rhs);
}

inline auto checksum_range(uint8_t const* begin, uint8_t const* end)
{
  uint8_t cs = 0x00;
  for(auto it=begin; it!=end; ++it)
    cs ^= *it;

  constexpr uint8_t b2h[] = "0123456789abcdef";
  return HexChecksum{
    .high_ = b2h[cs>>4],
    .low_  = b2h[cs&0x0f]
  };
}
}


inline bool add_checksum(Line& line)
{
  if(Line::max_size < line.size_ + 2)
    return false;

  auto const hcs = detail::checksum_range(line.data_.data(), line.data_.data() + line.size_);
  line.add_byte(hcs.high_);
  line.add_byte(hcs.low_);
  return true;
}


inline bool checksum_valid(Line const& line)
{
  if(line.size_ < 2u)
    return false;
  detail::HexChecksum const received{
    .high_ = line.data_[line.size_ - 2u],
    .low_  = line.data_[line.size_ - 1u]
  };
  if( received == detail::HexChecksum{} )
    return true;
  auto const ref = detail::checksum_range(line.data_.data(), line.data_.data() + line.size_ - 2u);
  return received == ref;
}

}
