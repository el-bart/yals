#pragma once
#include <optional>
#include <array>

namespace Io
{

struct Mtu
{
  static constexpr auto max_size = 1 + 2 + 2*16 + 2 + 1;
  uint8_t size_{0};
  std::array<uint8_t, max_size> data_;
};

struct Frame
{
  struct Header
  {
    uint8_t version_{0};
    uint8_t size_{0};
  };

  Header header_;
  std::array<uint8_t, 16> payload_{};
  uint8_t checksum_{};
};


Mtu encode(Frame const& f) const
{
  (void)f;
  return Mtu{}; // TODO
}

std::optional<Frame> decode(Mtu const& mtu)
{
  (void)mtu;
  return {};    // TODO
}

}
