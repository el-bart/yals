#pragma once
#include "Io/Line.hpp"
#include "Io/Proto/common.hpp"

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
  if(line.size_ != 1)
    return {};
  if(line.data_[0] != '?')
    return {};
  return Request{};
}

inline Line encode(Reply const& r)
{
  if(r.err_)
    return err_line(r.err_);
  return {};    // TODO
}

}
