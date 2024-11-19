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

}
