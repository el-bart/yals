#pragma once
#include "Io/Line.hpp"
#include "Io/Proto/common.hpp"

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

inline std::optional<Request> decode(Line const& line)
{
  if(line.size_ != 1)
    return {};
  if(line.data_[0] != '!')
    return {};
  return Request{};
}

inline Line encode(Reply const& r)
{
  if(r.err_)
    return err_line(r.err_);
  Line line;
  line.size_ = snprintf(reinterpret_cast<char*>(line.data_.data()), Line::max_size, "+%03d", r.pos_);
  return line;
}

}
