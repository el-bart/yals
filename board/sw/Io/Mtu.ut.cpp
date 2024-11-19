#include "catch2/catch.hpp"
#include "Io/Mtu.hpp"

using Io::Buffer;
using Io::Mtu;
using Io::extract_mtu;

namespace
{

TEST_CASE("Io::extract_mtu()")
{
  Buffer b;
  REQUIRE(b.size_ == 0u);

  SECTION("no data")
  {
    b.dive_add('!', '\n');
    CHECK( not extract_mtu(b) );
    CHECK(b.size_ == 0u);
  }

  SECTION("wrong start marker")
  {
    b.dive_add('a', 'b', '\n');
    CHECK( not extract_mtu(b) );
  }

  SECTION("wrong end marker")
  {
    b.dive_add('!', 'b', 'c');
    CHECK( not extract_mtu(b) );
  }

  SECTION("frame does not start at the begining")
  {
    b.dive_add('a', '!', 'c', '\n', 'e');
    const auto mtu = extract_mtu(b);
    REQUIRE(mtu);
    REQUIRE(mtu->size_ == 1u);
    CHECK(mtu->data_[0] == 'c');

    REQUIRE(b.size_ == 1u);
    CHECK(b.data_[0] == 'e');
  }

  SECTION("minimal frame gets extracted")
  {
    b.dive_add('!', 'b', '\n');
    const auto mtu = extract_mtu(b);
    CHECK(b.size_ == 0u);
    REQUIRE(mtu);
    REQUIRE(mtu->size_ == 1u);
    CHECK(mtu->data_[0] == 'b');
  }
  SECTION("minimal frame gets extracted")
  {
    b.dive_add('!', 'b', '\n', '!', 'c', '\n');
    const auto mtu = extract_mtu(b);
    CHECK(b.size_ == 3u);
    CHECK(b.data_[0] == '!');
    CHECK(b.data_[1] == 'c');
    CHECK(b.data_[2] == '\n');
    REQUIRE(mtu);
    REQUIRE(mtu->size_ == 1u);
    CHECK(mtu->data_[0] == 'b');
  }
}

}
