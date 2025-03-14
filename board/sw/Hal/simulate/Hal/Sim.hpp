#pragma once
#include <initializer_list>
#include <deque>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cinttypes>
#include <cmath>
#include "Utils/Config/settings.hpp"
#include "Hal/detail/EEPROM.hpp"

namespace Hal
{

struct Sim
{
  Sim()
  {
    EEPROM_.resize(256, 0xFF);
  }

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
  float position_{0.5};         // 0..1 of scale
  float LED_brightness_{1.0};   // 0..1 of power
  bool simulate_stall_{false};  // when enabled, position does not change
  uint64_t last_watchdog_reset_time_{0};    // current_time_ copy of last call to Watchdog::reset()
  bool watchdog_caused_reboot_{false};      // simulate reboot caused by watchdog

  // UART I/O
  std::deque<uint8_t> rx_;      // data sent via dev's UART
  std::deque<uint8_t> tx_;      // data received via dev's UART

  // EEPROM
  std::vector<uint8_t> EEPROM_;
  // marker
  void marker_set()   { EEPROM_[detail::EEPROM::address_marker] = detail::EEPROM::marker_set_value; }
  void marker_clear() { EEPROM_[detail::EEPROM::address_marker] = 0xFF; }
  bool marker_is_set() const { return EEPROM_[detail::EEPROM::address_marker] == detail::EEPROM::marker_set_value; }
  // min
  float min_position() const { return *reinterpret_cast<float const*>( EEPROM_.data() + detail::EEPROM::address_min_pos ); }
  void min_position(float value) {    *reinterpret_cast<float*      >( EEPROM_.data() + detail::EEPROM::address_min_pos ) = value; }
  // max
  float max_position() const { return *reinterpret_cast<float const*>( EEPROM_.data() + detail::EEPROM::address_max_pos ); }
  void max_position(float value) {    *reinterpret_cast<float*      >( EEPROM_.data() + detail::EEPROM::address_max_pos ) = value; }
  // brightness
  float EEPROM_LED_brightness() const { return *reinterpret_cast<float const*>( EEPROM_.data() + detail::EEPROM::address_LED ); }
  void EEPROM_LED_brightness(float value) {    *reinterpret_cast<float*      >( EEPROM_.data() + detail::EEPROM::address_LED ) = value; }

  // model parameters
  static constexpr uint64_t ticks_per_second_{1'000'000};

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
    if(simulate_stall_)
      if(engine_force_ < 50'000)
        return;
    // it's a very hand-wavy way to simulating engine torque to compute simulated
    // movement of the carriage. the "model" is also linear, as it's not really
    // good for any real testing. it's more of an 'example' approach.
    const auto f = engine_force_ / 65535.0f;
    const auto f_dt = f / Utils::Config::servo_full_path_travel_time_s;
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
