#include "catch2/catch.hpp"
#include "Io/Mtu.hpp"

using Io::Buffer;
using Io::Mtu;

namespace
{

TEST_CASE("Io::extract_mtu()")
{
  Buffer b;
  REQUIRE(b.size_ == 0u);

  SECTION("empty input yields nothing")
  {
    CHECK( not extract_mtu(b) );
    CHECK(b.size_ == 0u);
  }

  SECTION("dropping all non-frame-start bytes")
  {
    b.size_ = 4;
    b.data_[0] = 'a';
    b.data_[1] = 'b';
    b.data_[2] = 'c';
    b.data_[3] = 'd';
    CHECK( not extract_mtu(b) );
    CHECK(b.size_ == 0u);
  }

  SECTION("dropping non-frame-start bytes")
  {
    b.size_ = 4;
    b.data_[0] = 'a';
    b.data_[1] = 'b';
    b.data_[2] = '!';
    b.data_[3] = 0x02;  // this is a valid header
    b.data_[4] = 0x13;  // this is 'unset' value - not part of payload yet
    CHECK( not extract_mtu(b) );
    REQUIRE(b.size_ == 2u);
    CHECK(b.data_[0] == '!');
    CHECK(b.data_[1] == 0x02);
    CHECK(b.data_[2] != 0x13);  // make sure reading ends where it's supposed to
  }

  SECTION("decoding a minimal valid frame")
  {
                    
    b.size_ = 8;
    b.data_[0] = '!';
    // header
    b.data_[1] = '0';
    b.data_[2] = '0';
    // payload
    b.data_[3] = 'c';
    b.data_[4] = 'd';
    // checksum
    b.data_[5] = '9';
    b.data_[6] = 'a';
    b.data_[7] = '\n';
    auto const m = extract_mtu(b);
    CHECK(b.size_ == 0u);
    // MTU
    REQUIRE(m);
    REQUIRE(m->size_ == 1u);
    CHECK(m->data_[0] == 0xcd);
  }
}

}
