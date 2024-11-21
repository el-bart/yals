#pragma once
#include <initializer_list>
#include <deque>
#include <algorithm>
#include <cstdlib>
#include <cinttypes>
#include <cmath>

namespace Hal
{

struct Sim
{
  // this updated world simulation state, based on current presets, each time it's set
  void update(float dt_sec)
  {
    update_time(dt_sec);
    update_amps();
    update_pos(dt_sec);
  }

  void reset()
  {
    Sim tmp;
    std::swap(*this, tmp);
  }

  // values set based on current situation
  uint64_t current_time_{123};  // current time point (see: ticks_per_second_)
  int32_t engine_force_{0};     // -/+ is dir, abs value is a force (16-bit)
  float amps_{0};               // engine current [A]
  float vcc_{12.1};             // Vcc [V]
  float position_{0};           // 0..1 of scale
  float LED_brightness_{1.0};   // 0..1 of power

  // UART I/O
  std::deque<uint8_t> rx_;      // data sent via dev's UART
  std::deque<uint8_t> tx_;      // data received via dev's UART

  // EEPROM:
  uint32_t marker_{0x42};       // indicator of write location (0x42 == set)
  float min_position_{0.0};     // 0..1 of scale
  float max_position_{1.0};     // 0..1 of scale
  float EEPROM_LED_brightness_{1.0}; // 0..1 of power

  // model parameters
  static constexpr uint64_t ticks_per_second_{1'000'000};
  static constexpr auto eng_full_travel_time_s = 0.9f;

private:
  void update_time(float dt_sec)
  {
    current_time_ += ceil( dt_sec * ticks_per_second_ );
  }

  void update_amps()
  {
    amps_ = 0.01;
    if( engine_force_ == 0 )
      return;
    amps_ += abs(engine_force_) / 65535.0f * 0.5f;
  }

  void update_pos(float dt_sec)
  {
    // it's a very hand-wavy way to simulating engine torque to compute simulated
    // movement of the carriage. the "model" is also linear, as it's not really
    // good for any real testing. it's more of an 'example' approach.
    const auto f = engine_force_ / 65535.0f;
    const auto f_dt = f / eng_full_travel_time_s;
    const auto delta = f_dt * dt_sec;
    position_ = std::clamp(position_ + delta, 0.0f, 1.0f);
  }
};


inline Sim& sim()
{
  static Sim s;
  return s;
}

}
