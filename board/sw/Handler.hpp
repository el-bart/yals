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

  Io::Proto::Get_persistent_config::Reply handle(Io::Proto::Get_persistent_config::Request const& req)
  {
    // TODO
    return {
      .min_pos_ = 42,
      .max_pos_ = 142,
      .LED_brightness_ = 66
    };
  }

  Io::Proto::Get_servo_position::Reply handle(Io::Proto::Get_servo_position::Request const& req)
  {
    // TODO
    return { .pos_{73} };
  }

  Io::Proto::Get_telemetry::Reply handle(Io::Proto::Get_telemetry::Request const& req)
  {
    // TODO
    return {
      .eng_current_mA_ = 1234,
      .vcc_voltage_mV_ = 56789
    };
  }

  Io::Proto::Ping::Reply handle(Io::Proto::Ping::Request const& req)
  {
    // TODO: add SW version here
    return {};
  }

  Io::Proto::Set_LED_brightness::Reply handle(Io::Proto::Set_LED_brightness::Request const& req)
  {
    // TODO
    return {};
  }

  Io::Proto::Set_max_servo_position::Reply handle(Io::Proto::Set_max_servo_position::Request const& req)
  {
    // TODO
    return {};
  }

  Io::Proto::Set_min_servo_position::Reply handle(Io::Proto::Set_min_servo_position::Request const& req)
  {
    // TODO
    return {};
  }

  Io::Proto::Set_servo_position::Reply handle(Io::Proto::Set_servo_position::Request const& req)
  {
    // TODO
    return {};
  }

private:
  Context& ctx_;
};
