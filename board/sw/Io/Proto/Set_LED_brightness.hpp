#pragma once
#include "Io/Line.hpp"
#include "Io/Proto/common.hpp"

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

inline std::optional<Request> decode(Line const& line)
{
  if(line.size_ != 3)
    return {};
  if(line.data_[0] != '*')
    return {};
  auto const br = read_number( line.data_.data() + 1u, 2u );
  if(not br)
    return {};
  return Request{ .brightness_ = *br };
}

inline Line encode(Reply const& r)
{
  if(r.err_)
    return err_line(r.err_);
  Line line;
  line.size_ = snprintf(reinterpret_cast<char*>(line.data_.data()), Line::max_size, "+");
  return line;
}

}
