#include "catch2/catch.hpp"
#include "Io/Frame.hpp"

using Io::Mtu;
using Io::Frame;
using Io::Buffer;

namespace
{

TEST_CASE("Io::Frame adding bytes")
{
  Frame f;
  for(auto i=0u; i<Frame::max_size; ++i)
  {
    INFO("byte #" << i);
    REQUIRE( f.payload_add_byte(0x20 + i) );
  }
  CHECK( not f.payload_add_byte(0x13) );
  CHECK( f.header_.size_ == Frame::max_size );
}


TEST_CASE("Io::checksum() calculation")
{
  Frame f;
  uint8_t c = 0x00;
  CHECK( f.checksum() == c );

  c ^= 0x42;
  REQUIRE( f.payload_add_byte(0x42) );
  CHECK( f.checksum() == c );

  c ^= 0x62;
  REQUIRE( f.payload_add_byte(0x62) );
  CHECK( f.checksum() == c );
}


SCENARIO("Io::Frame decoding and encoding")
{
}

}
