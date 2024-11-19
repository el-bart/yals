#pragma once

namespace Io::Proto::Ping
{

struct Request
{
};

struct Reply
{
  // OK
  char const* pong_{nullptr};
  // error
  char const* err_{nullptr};
};

}
