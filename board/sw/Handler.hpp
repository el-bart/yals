#pragma once
#include "Io/process.hpp"
#include "Context.hpp"

struct Handler final
{
  explicit Handler(Context& ctx):
    ctx_{ctx}
  { }

  Handler(Handler const&) = delete;
  Handler& operator=(Handler const&) = delete;
  Handler(Handler&&) = delete;
  Handler& operator=(Handler&&) = delete;

  Get_persistent_config::Reply handle(Get_persistent_config::Request const& req)
  {
    // TODO
    return {
      .min_pos_ = 42,
      .max_pos_ = 142,
      .LED_brightness_ = 66
    };
  }

  Get_servo_position::Reply handle(Get_servo_position::Request const& req)
  {
    // TODO
    return { .pos_{73} };
  }

  Get_telemetry::Reply handle(Get_telemetry::Request const& req)
  {
    // TODO
    return {
      .eng_current_mA_ = 1234,
      .vcc_voltage_mV_ = 56789
    };
  }

  Ping::Reply handle(Ping::Request const& req)
  {
    // TODO
    return {};
  }

  Set_LED_brightness::Reply handle(Set_LED_brightness::Request const& req)
  {
    // TODO
    return {};
  }

  Set_max_servo_position::Reply handle(Set_max_servo_position::Request const& req)
  {
    // TODO
    return {};
  }

  Set_min_servo_position::Reply handle(Set_min_servo_position::Request const& req)
  {
    // TODO
    return {};
  }

  Set_servo_position::Reply handle(Set_servo_position::Request const& req)
  {
    // TODO
    return {};
  }

private:
  Context& ctx_;
};
