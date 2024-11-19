#pragma once
#include "Io/Line.hpp"

namespace Io::Proto::Set_LED_brightness
{

struct Request
{
  unsigned brightness_{};
};

struct Reply
{
  // OK
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
