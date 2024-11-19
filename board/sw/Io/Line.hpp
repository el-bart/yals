#pragma once
#include "Io/Buffer.hpp"
#include "Io/const.hpp"
#include "Io/utils.hpp"
#include <optional>
#include <array>
#include <cassert>

namespace Io
{

struct Line
{
  template<typename ...Args>
  bool add_byte(uint8_t b1, uint8_t b2, Args... args)
  {
    if( add_byte(b1) )
      if( add_byte(b2) )
        if( add_byte(args...) )
          return true;
    return false;
  }

  bool add_byte(uint8_t b)
  {
    if(size_ == max_size)
      return false;
    data_[size_] = b;
    ++size_;
    return true;
  }

  bool add_byte() { return true; }

  static constexpr size_t max_size = max_line_size;
  size_t size_{0};
  std::array<uint8_t, max_size> data_;
};


inline std::optional<Line> extract_line(Buffer& b)
{
  b.consume_leading_eols();
  if(b.size_ < 2u)  // at least 1 byte and and '\n' must be present here
    return {};

  auto const range = std::min(b.size_, Line::max_size + 1u);
  for(auto i = 0u; i < range; ++i)
    if( is_eol(b.data_[i]) )
    {
      auto const eol_pos = i;
      Line line;
      std::copy(b.data_.begin(), b.data_.begin() + eol_pos, line.data_.begin());
      line.size_ = eol_pos;
      b.trim_by(eol_pos + 1u);
      return line;
    }

  if(Line::max_size <= b.size_) // no command can be larger than a single line
    b.trim_by(1);
  return {};
}

}
