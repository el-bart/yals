#include "catch2/catch.hpp"
#include "Io/Line.hpp"

using Io::Buffer;
using Io::Line;
using Io::extract_line;

namespace
{

TEST_CASE("Io::extract_line()")
{
  Buffer b;
  REQUIRE(b.size_ == 0u);

  SECTION("no data")
  {
    b.dive_add('!', '\n');
    CHECK( not extract_line(b) );
    CHECK(b.size_ == 0u);
  }

  SECTION("wrong start marker")
  {
    b.dive_add('a', 'b', '\n');
    CHECK( not extract_line(b) );
  }

  SECTION("wrong end marker")
  {
    b.dive_add('!', 'b', 'c');
    CHECK( not extract_line(b) );
  }

  SECTION("frame does not start at the begining")
  {
    b.dive_add('a', '!', 'c', '\n', 'e');
    const auto line = extract_line(b);
    REQUIRE(line);
    REQUIRE(line->size_ == 1u);
    CHECK(line->data_[0] == 'c');

    REQUIRE(b.size_ == 1u);
    CHECK(b.data_[0] == 'e');
  }

  SECTION("minimal frame gets extracted")
  {
    b.dive_add('!', 'b', '\n');
    const auto line = extract_line(b);
    CHECK(b.size_ == 0u);
    REQUIRE(line);
    REQUIRE(line->size_ == 1u);
    CHECK(line->data_[0] == 'b');
  }
  SECTION("minimal frame gets extracted")
  {
    b.dive_add('!', 'b', '\n', '!', 'c', '\n');
    const auto line = extract_line(b);
    CHECK(b.size_ == 3u);
    CHECK(b.data_[0] == '!');
    CHECK(b.data_[1] == 'c');
    CHECK(b.data_[2] == '\n');
    REQUIRE(line);
    REQUIRE(line->size_ == 1u);
    CHECK(line->data_[0] == 'b');
  }
}

}
