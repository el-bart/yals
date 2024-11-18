#pragma once
#include "Hal/Sim.hpp"
#include <optional>
#include <cstring>

namespace Hal::Impl
{

struct Uart
{
  Uart() = default;

  Uart(Uart const&) = delete;
  Uart& operator=(Uart const&) = delete;
  Uart(Uart &&) = delete;
  Uart& operator=(Uart &&) = delete;

  bool tx(uint8_t b)
  {
    if( sim().tx_.size() >= 32u )
      return false;
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
