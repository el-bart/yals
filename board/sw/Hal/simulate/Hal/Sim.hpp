#pragma once
#include <initializer_list>
#include <deque>
#include <algorithm>
#include <cstdlib>
#include <cinttypes>

namespace Hal
{

struct Sim
{
  // this updated world simulation state, based on current presets, each time it's set
  void update(float dt_sec)
  {
    update_amps();
    update_pos(dt_sec);
  }

  void reset()
  {
    Sim tmp;
    std::swap(*this, tmp);
  }

  // values set based on current situation
  int32_t engine_force_{0}; // -/+ is dir, abs value is a force (16-bit)
  float amps_{0};           // engine current [A]
  float vcc_{12.1};         // Vcc [V]
  float position_{0};       // 0..1 of scale

  // UART I/O
  std::deque<uint8_t> rx_;  // data sent via dev's UART
  std::deque<uint8_t> tx_;  // data received via dev's UART

  // EEPROM:
  uint32_t marker_{0x42};   // indicator of write location (0x42 == set)
  float min_position_{0.0}; // 0..1 of scale
  float max_position_{1.0}; // 0..1 of scale
  float LED_brightness_{0}; // 0..1 of power

private:
  void update_amps()
  {
    amps_ = 0.01;
    if( engine_force_ == 0 )
      return;
    amps_ += abs(engine_force_) / 65535.0f * 0.5f;
  }

  void update_pos(float dt_sec)
  {
    const auto f = engine_force_ / 65535.0f;
    constexpr auto full_travel_sec = 2.0f;
    const auto distance = f / full_travel_sec;
    const auto delta = distance * dt_sec;
    position_ = std::clamp(position_ + delta, 0.0f, 1.0f);
  }
};


inline Sim& sim()
{
  static Sim s;
  return s;
}

}
