#pragma once

namespace Io::Proto::Set_servo_position
{

struct Request
{
  unsigned pos_{};
};

struct Reply
{
  // OK
  // error
  char const* err_{nullptr};
};

}
