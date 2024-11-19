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
    CHECK( not extract_line(b) );
    CHECK(b.size_ == 0u);
  }

  SECTION("no real data")
  {
    b.dive_add('\r', '\n');
    CHECK( not extract_line(b) );
    CHECK(b.size_ == 0u);
  }

  SECTION("no end marker")
  {
    b.dive_add('a', 'b', 'c');
    CHECK( not extract_line(b) );
    CHECK(b.size_ == 3u);
  }

  SECTION("frame does not start at the begining")
  {
    b.dive_add('\n', '\r', 'c', '\n', 'e');
    const auto line = extract_line(b);
    REQUIRE(line);
    REQUIRE(line->size_ == 1u);
    CHECK(line->data_[0] == 'c');

    REQUIRE(b.size_ == 1u);
    CHECK(b.data_[0] == 'e');
  }

  SECTION("minimal frame gets extracted")
  {
    b.dive_add('a', '\n');
    const auto line = extract_line(b);
    CHECK(b.size_ == 0u);
    REQUIRE(line);
    REQUIRE(line->size_ == 1u);
    CHECK(line->data_[0] == 'a');
  }

  SECTION("minimal frame gets extracted and next command remains in buffer")
  {
    b.dive_add('a', '\n', 'b', '\n');
    const auto line = extract_line(b);
    CHECK(b.size_ == 2u);
    CHECK(b.data_[0] == 'b');
    CHECK(b.data_[1] == '\n');
    REQUIRE(line);
    REQUIRE(line->size_ == 1u);
    CHECK(line->data_[0] == 'a');
  }

  SECTION("attempting extraction on buffer where no EOL is within max line length drops one byte")
  {
    for(auto i=0; i<Line::max_size; ++i)
      b.dive_add('a' + i);
    b.dive_add('!');
    REQUIRE( b.size_ == Line::max_size + 1 );

    REQUIRE( not extract_line(b) );
    CHECK( b.size_ == Line::max_size );
  }

  SECTION("extraction won't happen for a too long line")
  {
    for(auto i=0; i<Line::max_size; ++i)
      b.dive_add('a' + i);
    b.dive_add('\n');
    REQUIRE( b.size_ == Line::max_size + 1 );

    REQUIRE( not extract_line(b) );
    CHECK( b.size_ == Line::max_size );

    auto const line = extract_line(b);
    REQUIRE(line);
    CHECK(line->size_ == Line::max_size);
  }
}

}
