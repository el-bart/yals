#pragma once
#include "Io/const.hpp"
#include "Io/utils.hpp"
#include <optional>
#include <array>

namespace Io
{

// byte-buffer, as received from (or to be sent via) serial port
struct Buffer final
{
  template<typename ...Args>
  void dive_add(uint8_t b1, uint8_t b2, Args... args)
  {
    dive_add(b1);
    dive_add(b2);
    dive_add(args...);
  }

  void dive_add(uint8_t b)
  {
    if(size_ == max_size)
      trim_by(1);
    data_[size_] = b;
    ++size_;
  }

  void dive_add() { }

  void consume_leading_eols()
  {
    if(size_ == 0u)
      return;
    auto const count = leading_eols_count();
    trim_by(count);
  }

  void trim_by(uint8_t trim_size)
  {
    if(size_ < trim_size)
    {
      size_ = 0;
      return;
    }
    auto const new_size = size_ - trim_size;
    for(auto i=0u; i<new_size; ++i)
      data_[i] = data_[trim_size + i];
    size_ = new_size;
  }

  static constexpr size_t max_size = max_buffer_size;
  size_t size_{0};
  std::array<uint8_t, max_size> data_;

private:
  size_t leading_eols_count() const
  {
    size_t c = 0;
    for(; c<size_; ++c)
      if( not is_eol(data_[c]) )
        return c;
    return c;
  }
};

}
