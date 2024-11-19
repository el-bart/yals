#pragma once

namespace Io::Proto::Set_max_servo_position
{

struct Request
{
  unsigned max_pos_{};
};

struct Reply
{
  // OK
  // error
  char const* err_{nullptr};
};

}
