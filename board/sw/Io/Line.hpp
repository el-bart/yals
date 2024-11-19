#pragma once
#include "Io/Buffer.hpp"
#include "Io/const.hpp"
#include <optional>
#include <array>
#include <cassert>

namespace Io
{

struct Line
{
  static constexpr uint8_t max_size = max_line_size;
  uint8_t size_{0};
  std::array<uint8_t, max_size> data_;
};


namespace detail
{
inline std::optional<Line> to_line(Buffer const& b, uint8_t eol_pos)
{
  if(b.size_ <= eol_pos)
    return {};
  if(eol_pos < 1u)  // not even 1 byte - dropping
    return {};
  assert(b.data_[eol_pos] == '\n');
  return line;
}
}


inline std::optional<Line> extract_line(Buffer& b)
{
  if(b.size_ < 2u)  // at least 1 byte and and '\n' must be present here
    return {};
  for(auto eol_pos = 0u; eol_pos < b.size_; ++eol_pos)
    if(b.data_[i] == '\n')
    {
      Line line;
      std::copy(b.data_.begin() + 1u, b.data_.begin() + eol_pos, line.data_.begin());
      line.size_ = eol_pos - 1u;
      auto const line = detail::to_line(b, i);
      b.trim_by(i + 1u);
      return line;
    }
  return {};
}

}
