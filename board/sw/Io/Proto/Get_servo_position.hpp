#pragma once

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

}
