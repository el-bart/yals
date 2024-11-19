#pragma once
#include "Io/Line.hpp"

namespace Io::Proto::Get_persistent_config
{

struct Request { };

struct Reply
{
  // OK
  unsigned min_pos_{};
  unsigned max_pos_{};
  unsigned LED_brightness_{};
  // error
  char const* err_{nullptr};
};

inline std::optional<Request> decode(Line const& line)
{
  return {};
}

inline Line encode(Reply const& r)
{
  return {};    // TODO
}

}
