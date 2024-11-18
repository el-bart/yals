#include "Hal/Engine.hpp"
#include <initializer_list>

auto constexpr delay = 2000;

using E = Hal::Engine;

int main()
{
  E eng;
  while(true)
    for(auto d: { E::Direction::Left, E::Direction::Off, E::Direction::Right, E::Direction::Off })
      for(auto f: { 0.00, /*0.20,*/ 0.40, 0.50, 0.60, 0.80, 1.00 })
      {
        eng.force( f * 65536);
        eng.direction(d);
        sleep_ms(delay);
        if( d == E::Direction::Off )
          break;
      }
}
