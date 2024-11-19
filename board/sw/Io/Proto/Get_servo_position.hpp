#pragma once
#include "Io/Line.hpp"

namespace Io::Proto::Get_servo_position
{

struct Request { };

struct Reply
{
  // OK
  unsigned pos_{};
  // error
  char const* err_{nullptr};
};

inline Request decode(Line const& line)
{
  return {};    // TODO
}

inline Line encode(Reply const& r)
{
  return {};    // TODO
}

}
