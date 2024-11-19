#pragma once
#include "Io/Line.hpp"
#include "Io/checksum.hpp"
#include "Io/Proto/Get_persistent_config.hpp"
#include "Io/Proto/Get_servo_position.hpp"
#include "Io/Proto/Get_telemetry.hpp"
#include "Io/Proto/Ping.hpp"
#include "Io/Proto/Set_LED_brightness.hpp"
#include "Io/Proto/Set_max_servo_position.hpp"
#include "Io/Proto/Set_min_servo_position.hpp"
#include "Io/Proto/Set_servo_position.hpp"
#include <optional>

namespace Io
{

namespace detail
{
template<size_t N>
auto error_line(char const (&str)[N])
{
  static_assert( 1u + N-1u + 2u <= Line::max_size, "error message will not fit the buffer" );
  Line err;
  err.add_byte('-');
  for(auto i=0u; i<N-1u; ++i)
    err.add_byte( str[i] );
  return err;
}

template<size_t N>
auto error_line_with_checksum(char const (&str)[N])
{
  auto err = error_line(str);
  add_checksum(err);
  return err;
}

template<typename Dec, typename Enc, typename H>
auto run_handler(H&& h, Line const& line, Dec&& dec, Enc&& enc)
{
  auto const req = dec(line);
  if(not req)
    return detail::error_line("cmd err");
  return enc( h.handle(*req) );
}

template<typename H>
auto dispatch(Line const& line, H&& h)
{
  using namespace Proto;

  switch(line.data_[0])
  {
    case '?': return run_handler(h, line, Get_persistent_config::decode, Get_persistent_config::encode);
    case '!': return run_handler(h, line, Get_servo_position::decode, Get_servo_position::encode);
  }

  return detail::error_line("unknown cmd");
}
}

template<typename H>
Line process(Line line, H&& h)
{
  if(line.size_ < 3u) // definitely not valid
    return detail::error_line_with_checksum("too short");

  if( not checksum_valid(line) )
    return detail::error_line_with_checksum("invalid checksum");
  line.size_ -= 2u;     // trim checksum

  auto out = detail::dispatch(line, h);
  add_checksum(out);
  return out;
}

}
