#pragma once

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

}
