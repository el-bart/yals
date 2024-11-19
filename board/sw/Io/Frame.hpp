#pragma once
#include "Io/Mtu.hpp"
#include "Io/Buffer.hpp"
#include "Io/const.hpp"
#include <optional>
#include <array>

namespace Io
{

// logical representation of a frame (with binary data)
struct Frame
{
  struct Header
  {
    uint8_t version_{0};
    uint8_t size_{0};
  };

  bool payload_add_byte(uint8_t b)
  {
    if(header_.size_ == max_size)
      return false;
    payload_[header_.size_] = b;
    ++header_.size_;
    return true;
  }

  uint8_t checksum() const
  {
    uint8_t c = 0x00;
    for(auto i=0u; i<header_.size_; ++i)
      c ^= payload_[i];
    return c;
  }

  static constexpr uint8_t max_size = max_frame_size;
  Header header_;
  std::array<uint8_t, max_size> payload_{};
};


namespace detail
{
struct HexByte
{
  char high_{};
  char low_{};
};

inline bool operator==(HexByte const a, HexByte const b) { return a.high_ == b.high_ && a.low_ == b.low_; }
inline bool operator!=(HexByte const a, HexByte const b) { return not ( a == b ); }

inline auto halfbyte_to_hex(uint8_t b) { return "0123456789abcdef"[ b & 0x0f ]; }

inline auto to_hex(uint8_t b)
{
  return HexByte{ .high_ = halfbyte_to_hex(b>>4), .low_ = halfbyte_to_hex(b&0x0f) };
}

inline std::optional<uint8_t> hex_to_byte(char c)
{
  if('0' <= c && c <='9')
    return c - '0';
  if('a' <= c && c <='f')
    return 10 + c - 'a';
  if('A' <= c && c <='F')
    return 10 + c - 'A';
  return {};
}

inline std::optional<uint8_t> to_byte(HexByte hb)
{
  auto const h = hex_to_byte(hb.high_);
  auto const l = hex_to_byte(hb.low_);
  if(not h || not l)
    return {};
  uint8_t const v = ( *h << 4 ) | *l;
  return v;
}
} // unnamed namespace


inline std::optional<Mtu> encode(Frame const& f)
{
  (void)f;
  return {};    // TODO
}

inline std::optional<Frame> decode(Mtu const& m)
{
  return {};    // TODO
}

}
