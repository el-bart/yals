#pragma once
#include "Hal/Impl/sleep.hpp"
#include <chrono>

namespace Hal
{

inline void sleep(std::chrono::microseconds const us) { Impl::sleep(us); }
inline void sleep(std::chrono::milliseconds const ms) { Impl::sleep(ms); }
inline void sleep(std::chrono::seconds const s)       { Impl::sleep(s);  }

}
