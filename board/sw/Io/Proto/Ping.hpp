#pragma once
#include "Io/Line.hpp"

namespace Io::Proto::Ping
{

struct Request { };

struct Reply
{
  // OK
  char const* pong_{nullptr};
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
