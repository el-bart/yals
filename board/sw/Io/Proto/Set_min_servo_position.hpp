#pragma once
#include "Io/Line.hpp"
#include "Io/Proto/common.hpp"

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
  if(line.size_ != 4)
    return {};
  if(line.data_[0] != '<')
    return {};
  auto const mp = read_number( line.data_.data() + 1u, 3u );
  if(not mp)
    return {};
  return Request{ .min_pos_ = *mp };
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
