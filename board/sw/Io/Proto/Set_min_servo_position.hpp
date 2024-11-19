#pragma once

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

}
