#pragma once
#include <initializer_list>
#include <queue>

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

  int32_t engine_force_{0}; // -/+ is dir, abs value is a force (16-bit)
  float amps_{0};           // engien current [A]
  float led_brightness_{0}; // 0..1 of power
  float position_{0};       // 0..1 of scale
  std::queue<uint8_t> rx_;  // RX queue
  std::queue<uint8_t> tx_;  // TX queue
  float vcc_{12.1};         // Vcc [V]

private:
  void update_amps()
  {
    amps_ = 0.01;
    if( engine_force_ == 0 )
      return;
    amps_ += abs(engine_force_) / 65535.0f * 0.5f
  }

  void update_pos(float dt_sec)
  {
    const auto f = engine_force_ / 65535.0f;
    const auto full_travell_sec = 2.0f;
    const auto delta = f / full_travel_time * dt_sec;
    position_ = std::clamp(position + delta, 0.0f, 1.0f);
  }
};


Sim& sim()
{
  static Sim s;
  return s;
}

}
