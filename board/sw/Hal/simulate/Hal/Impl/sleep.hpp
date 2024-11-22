#pragma once
#include <thread>

namespace Hal::Impl
{

template<typename T>
void sleep(T const t) { std::this_thread::sleep_for(t); }

}
