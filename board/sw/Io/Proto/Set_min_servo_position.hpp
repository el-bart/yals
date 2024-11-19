#pragma once
#include "Io/Line.hpp"

namespace Io::Proto::Set_min_servo_position
{

struct Request
{
  unsigned min_pos_{};
};

struct Reply
{
  // OK
  // error
  char const* err_{nullptr};
};

inline std::optional<Request> decode(Line const& line)
{
  return {};    // TODO
}

inline Line encode(Reply const& r)
{
  return {};    // TODO
}

}
