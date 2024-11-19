#include "catch2/catch.hpp"
#include "Io/Proto/common.hpp"

using Io::Proto::err_line;
using Io::Proto::read_number;

namespace
{

TEST_CASE("Io::Proto::err_line()")
{
  SECTION("empty line")
  {
    auto const line = err_line("");
    REQUIRE(line.size_ == 1);
    CHECK(line.data_[0] == '-');
  }

  SECTION("sample str")
  {
    auto const line = err_line("abc");
    REQUIRE(line.size_ == 4);
    CHECK(line.data_[0] == '-');
    CHECK(line.data_[1] == 'a');
    CHECK(line.data_[2] == 'b');
    CHECK(line.data_[3] == 'c');
  }
}


auto valid(std::optional<unsigned> opt)
{
  if(not opt)
    throw std::runtime_error{"expected value is not set"};
  return *opt;
}

TEST_CASE("Io::Proto::read_numer()")
{
  uint8_t const str[] = { '0', '1', '2', '3' };
  CHECK( not read_number(str, 0) );
  CHECK( valid( read_number(str, 1) ) ==   0 );
  CHECK( valid( read_number(str, 2) ) ==   1 );
  CHECK( valid( read_number(str, 3) ) ==  12 );
  CHECK( valid( read_number(str, 4) ) == 123 );
}

}
