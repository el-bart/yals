#pragma once
#include "Io/Line.hpp"

namespace Io::Proto::Get_telemetry
{

struct Request { };

struct Reply
{
  // OK
  unsigned eng_current_mA_{};
  unsigned vcc_voltage_mV_{};
  // error
  char const* err_{nullptr};
};

inline std::optional<Request> decode(Line const& line)
{
  return {};    // TODO
}

inline Line encode(Reply const& r)
{
  return {};    // TODO
}

}
