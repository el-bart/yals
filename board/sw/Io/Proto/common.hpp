#pragma once
#include "Io/Line.hpp"

namespace Io::Proto
{

inline Line err_line(char const* err)
{
  Line line;
  line.add_byte('-');
  for(; *err!=0; ++err)
    line.add_byte(*err);
  return line;
}


inline std::optional<unsigned> read_number(uint8_t const* data, size_t size)
{
  if(size == 0u)
    return {};
  auto constexpr is_num = +[](uint8_t b) { return '0' <= b && b <= '9'; };
  unsigned out = 0;
  for(auto it=data; it!=data+size; ++it)
  {
    if( not is_num(*it) )
      return {};
    out *= 10u;
    out += (*it - '0');
  }
  return out;
}

}
