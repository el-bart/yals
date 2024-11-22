#pragma once
#include <chrono>
#include <pico/stdlib.h>

namespace Hal::Impl
{

inline void sleep(std::chrono::microseconds const us) { ::sleep_us( us.count() ); }
inline void sleep(std::chrono::milliseconds const ms) { ::sleep_ms( ms.count() ); }
inline void sleep(std::chrono::seconds const s)       { sleep( std::chrono::milliseconds{ 1'000 * s.count() } ); }

}
