#include "catch2/catch.hpp"
#include "Io/Frame.hpp"

using Io::Mtu;
using Io::Frame;
using Io::Buffer;
using Io::detail::HexByte;
using Io::detail::to_hex;
using Io::detail::to_byte;

namespace
{

TEST_CASE("Io::detail::to_byte")
{
  // low
  CHECK( *to_byte( HexByte{'0', '0'} ) == 0x00 );
  CHECK( *to_byte( HexByte{'0', '1'} ) == 0x01 );
  CHECK( *to_byte( HexByte{'0', '2'} ) == 0x02 );
  CHECK( *to_byte( HexByte{'0', '3'} ) == 0x03 );
  CHECK( *to_byte( HexByte{'0', '4'} ) == 0x04 );
  CHECK( *to_byte( HexByte{'0', '5'} ) == 0x05 );
  CHECK( *to_byte( HexByte{'0', '6'} ) == 0x06 );
  CHECK( *to_byte( HexByte{'0', '7'} ) == 0x07 );
  CHECK( *to_byte( HexByte{'0', '8'} ) == 0x08 );
  CHECK( *to_byte( HexByte{'0', '9'} ) == 0x09 );
  CHECK( *to_byte( HexByte{'0', 'a'} ) == 0x0a );
  CHECK( *to_byte( HexByte{'0', 'b'} ) == 0x0b );
  CHECK( *to_byte( HexByte{'0', 'c'} ) == 0x0c );
  CHECK( *to_byte( HexByte{'0', 'd'} ) == 0x0d );
  CHECK( *to_byte( HexByte{'0', 'e'} ) == 0x0e );
  CHECK( *to_byte( HexByte{'0', 'f'} ) == 0x0f );
  // high
  CHECK( *to_byte( HexByte{'0', '0'} ) == 0x00 );
  CHECK( *to_byte( HexByte{'1', '0'} ) == 0x10 );
  CHECK( *to_byte( HexByte{'2', '0'} ) == 0x20 );
  CHECK( *to_byte( HexByte{'3', '0'} ) == 0x30 );
  CHECK( *to_byte( HexByte{'4', '0'} ) == 0x40 );
  CHECK( *to_byte( HexByte{'5', '0'} ) == 0x50 );
  CHECK( *to_byte( HexByte{'6', '0'} ) == 0x60 );
  CHECK( *to_byte( HexByte{'7', '0'} ) == 0x70 );
  CHECK( *to_byte( HexByte{'8', '0'} ) == 0x80 );
  CHECK( *to_byte( HexByte{'9', '0'} ) == 0x90 );
  CHECK( *to_byte( HexByte{'a', '0'} ) == 0xa0 );
  CHECK( *to_byte( HexByte{'b', '0'} ) == 0xb0 );
  CHECK( *to_byte( HexByte{'c', '0'} ) == 0xc0 );
  CHECK( *to_byte( HexByte{'d', '0'} ) == 0xd0 );
  CHECK( *to_byte( HexByte{'e', '0'} ) == 0xe0 );
  CHECK( *to_byte( HexByte{'f', '0'} ) == 0xf0 );
  // upper case
  CHECK( *to_byte( HexByte{'A', 'B'} ) == 0xAB );
  CHECK( *to_byte( HexByte{'C', 'D'} ) == 0xCD );
  CHECK( *to_byte( HexByte{'E', 'F'} ) == 0xEF );
  // invalid
  CHECK( not to_byte( HexByte{'x', '0'} ) );
  CHECK( not to_byte( HexByte{'0', 'Q'} ) );
}


TEST_CASE("Io::Frame::payload_add_byte()")
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


TEST_CASE("Io::checksum()")
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


TEST_CASE("Io::Frame decoding and encoding")
{
  Frame f;

  SECTION("serializing empty frame")
  {
    CHECK( not encode(f) );
  }

  SECTION("serializing 1-byte frame")
  {
    f.payload_add_byte(0x13);
    auto const m = encode(f);
    REQUIRE(m);
    REQUIRE(m->size_ == 1 + 2 + 2 + 1);
    CHECK(m->data_[0] == '!');
    CHECK(m->data_[1] == '0');
    CHECK(m->data_[2] == '0');
    CHECK(m->data_[3] == '1');
    CHECK(m->data_[4] == '3');
    CHECK(m->data_[5] == '1');
    CHECK(m->data_[6] == '3');
    CHECK(m->data_[7] == '\n');
  }
}

}
