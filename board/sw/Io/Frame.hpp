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
