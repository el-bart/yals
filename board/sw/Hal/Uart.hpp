#pragma once
#include "Hal/Impl/Uart.hpp"
#include <optional>
#include <cstring>

namespace Hal
{

struct Uart
{
  Uart() = default;

  size_t tx(char const* str)
  {
    return tx(reinterpret_cast<uint8_t const*>(str), strlen(str));
  }

  size_t tx(uint8_t const* buf, size_t size)
  {
    for(size_t i=0; i<size; ++i)
      if(not tx(buf[i]))
        return i;
    return size;
  }

  bool tx(uint8_t b)
  {
    return impl_.tx(b);
  }

  std::optional<uint8_t> rx()
  {
    return impl_.rx();
  }

private:
  Impl::Uart impl_;
};

}
