#include "catch2/catch.hpp"
#include "Io/checksum.hpp"

using Io::Line;
using Io::add_checksum;

namespace
{

TEST_CASE("Io::add_checksum()")
{
  Line line;
  REQUIRE(line.size_ == 0u);

  SECTION("empty line")
  {
    REQUIRE( add_checksum(line) );
    REQUIRE(line.size_ == 2u);
    CHECK(line.data_[0] == '0');
    CHECK(line.data_[1] == '0');
  }

  SECTION("one character")
  {
    line.add_byte('a');
    REQUIRE( add_checksum(line) );
    REQUIRE(line.size_ == 3u);
    CHECK(line.data_[0] == 'a');
    CHECK(line.data_[1] == '6');
    CHECK(line.data_[2] == '1');
  }

  SECTION("two characters")
  {
    line.add_byte('a');
    line.add_byte('b');
    REQUIRE( add_checksum(line) );
    REQUIRE(line.size_ == 4u);
    CHECK(line.data_[0] == 'a');
    CHECK(line.data_[1] == 'b');
    CHECK(line.data_[2] == '0');
    CHECK(line.data_[3] == '3');
  }

  SECTION("cannot add checksum when it would result in too long line")
  {
    for(auto i = 0u; i < Line::max_size - 1u; ++i)
      line.add_byte('a' + i);
    REQUIRE(line.size_ == Line::max_size - 1u);

    CHECK( not add_checksum(line) );
    CHECK(line.size_ == Line::max_size - 1u);
  }

  SECTION("can add checksum when it will fit into a line")
  {
    for(auto i = 0u; i < Line::max_size - 2u; ++i)
      line.add_byte('a' + i);
    REQUIRE(line.size_ == Line::max_size - 2u);

    CHECK( add_checksum(line) );
    CHECK(line.size_ == Line::max_size);
  }
}


TEST_CASE("Io::checksum_valid()")
{
  Line line;
  REQUIRE(line.size_ == 0u);

  SECTION("empty line")
  {
    CHECK( not checksum_valid(line) );
  }

  SECTION("1B line")
  {
    line.add_byte('a');
    CHECK( not checksum_valid(line) );
  }

  SECTION("empty line with checksum only")
  {
    REQUIRE( add_checksum(line) );
    CHECK( checksum_valid(line) );
  }

  SECTION("one character")
  {
    line.add_byte('a');
    REQUIRE( add_checksum(line) );
    CHECK( checksum_valid(line) );
  }

  SECTION("two characters")
  {
    line.add_byte('a');
    line.add_byte('b');
    REQUIRE( add_checksum(line) );
    CHECK( checksum_valid(line) );
  }

  SECTION("unset checksum is considered valid")
  {
    line.add_byte('a');
    line.add_byte('b');
    line.add_byte('X');
    line.add_byte('X');
    CHECK( checksum_valid(line) );
  }

  SECTION("invalid encoded checksum is not valid #1")
  {
    line.add_byte('a');
    line.add_byte('#');
    line.add_byte('f');
    CHECK( not checksum_valid(line) );
  }

  SECTION("invalid encoded checksum is not valid #2")
  {
    line.add_byte('a');
    line.add_byte('f');
    line.add_byte('#');
    CHECK( not checksum_valid(line) );
  }

  SECTION("1 bit flip invalidates checksum")
  {
    line.add_byte('a');
    line.add_byte('b');
    REQUIRE( add_checksum(line) );
    REQUIRE( checksum_valid(line) );
    line.data_[1] ^= 0x02;
    CHECK( not checksum_valid(line) );
  }
}

}
