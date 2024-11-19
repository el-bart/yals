#include "catch2/catch.hpp"
#include "Io/Buffer.hpp"

using Io::Buffer;

namespace
{

TEST_CASE("Io::Buffer")
{
  Buffer b;
  REQUIRE(b.size_ == 0u);

  SECTION("empty input trimming")
  {
    b.consume_leading_garbage();
    CHECK(b.size_ == 0u);
  }

  SECTION("dropping all non-frame-start bytes")
  {
    b.size_ = 4;
    b.data_[0] = 'a';
    b.data_[1] = 'b';
    b.data_[2] = 'c';
    b.data_[3] = 'd';
    b.consume_leading_garbage();
    CHECK(b.size_ == 0u);
  }

  SECTION("trimming start bytes")
  {
    b.size_ = 4;
    b.data_[0] = 'a';
    b.data_[1] = 'b';
    b.data_[2] = 'c';
    b.data_[3] = 'd';
    b.trim_by(2);
    CHECK(b.size_ == 2u);
    CHECK(b.data_[0] == 'c');
    CHECK(b.data_[1] == 'd');
  }

  SECTION("trimming all bytes")
  {
    b.size_ = 4;
    b.data_[0] = 'a';
    b.data_[1] = 'b';
    b.data_[2] = 'c';
    b.data_[3] = 'd';
    b.trim_by(4);
    CHECK(b.size_ == 0u);
  }

  SECTION("trimming more bytes than available")
  {
    b.size_ = 4;
    b.data_[0] = 'a';
    b.data_[1] = 'b';
    b.data_[2] = 'c';
    b.data_[3] = 'd';
    b.trim_by(5);
    CHECK(b.size_ == 0u);
  }

  SECTION("adding bytes")
  {
    b.dive_add(0x13);
    REQUIRE(b.size_ == 1u);
    CHECK(b.data_[0] == 0x13);

    b.dive_add(0x24);
    REQUIRE(b.size_ == 2u);
    CHECK(b.data_[0] == 0x13);
    CHECK(b.data_[1] == 0x24);
  }

  SECTION("adding bytes over buffer")
  {
    for(auto i=0u; i<Buffer::max_size; ++i)
      b.dive_add(0x42);
    REQUIRE(b.size_ == Buffer::max_size);
    CHECK(b.data_[0] == 0x42);
    CHECK(b.data_[b.size_ - 1] == 0x42);

    b.dive_add(0x66);
    REQUIRE(b.size_ == Buffer::max_size);
    CHECK(b.data_[0] == 0x42);
    CHECK(b.data_[b.size_ - 1] == 0x66);
  }

  SECTION("adding multiple bytes")
  {
    b.dive_add(0x13, 0x24, 0x66);
    REQUIRE(b.size_ == 3u);
    CHECK(b.data_[0] == 0x13);
    CHECK(b.data_[1] == 0x24);
    CHECK(b.data_[2] == 0x66);
  }

  SECTION("adding multiple bytes over buffer")
  {
    for(auto i=0u; i<Buffer::max_size; ++i)
      b.dive_add(0x42);
    REQUIRE(b.size_ == Buffer::max_size);
    CHECK(b.data_[0] == 0x42);
    CHECK(b.data_[b.size_ - 1] == 0x42);

    b.dive_add(0x13, 0x24);
    REQUIRE(b.size_ == Buffer::max_size);
    CHECK(b.data_[b.size_ - 2] == 0x13);
    CHECK(b.data_[b.size_ - 1] == 0x24);
  }
}

}
