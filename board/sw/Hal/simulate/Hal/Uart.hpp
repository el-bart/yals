#pragma once
#include "Hal/Sim.hpp"
#include <optional>
#include <cstring>

namespace Hal
{

struct Uart
{
  Uart() = default;

  Uart(Uart const&) = delete;
  Uart& operator=(Uart const&) = delete;
  Uart(Uart &&) = delete;
  Uart& operator=(Uart &&) = delete;

  size_t tx(char const* str)
  {
    return tx(reinterpret_cast<uint8_t const*>(str), strlen(str));
  }

  size_t tx(uint8_t const* buf, size_t size)
  {
    for(size_t i=0; i<size; ++i)
      tx(buf[i]);
    return size;
  }

  bool tx(uint8_t b)
  {
    sim().tx_.push(b);
    return true;
  }

  std::optional<uint8_t> rx()
  {
    if( sim().rx_.empty() )
      return {};
    auto b = sim().rx_.front();
    sim().rx_.pop();
    return b;
  }
};

}
