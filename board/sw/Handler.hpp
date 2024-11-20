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
    return {
      .min_pos_ = static_cast<uint32_t>( roundf(ctx_.setpoints_.min_pos_ * 999) ),
      .max_pos_ = static_cast<uint32_t>( roundf(ctx_.setpoints_.max_pos_ * 999) ),
      .LED_brightness_ = static_cast<uint32_t>( roundf(ctx_.setpoints_.LED_brightness_ * 99) )
    };
  }

  Io::Proto::Get_servo_position::Reply handle(Io::Proto::Get_servo_position::Request const& req)
  {
    return { .pos_ = static_cast<uint32_t>( roundf(ctx_.last_reads_.position_ * 999) ) };
  }

  Io::Proto::Get_telemetry::Reply handle(Io::Proto::Get_telemetry::Request const& req)
  {
    return {
      .eng_current_mA_ = static_cast<uint32_t>( roundf(ctx_.last_reads_.engine_current_A_ * 1000.0f ) ),
      .vcc_voltage_mV_ = static_cast<uint32_t>( roundf(ctx_.last_reads_.vcc_V_            * 1000.0f ) ),
    };
  }

  Io::Proto::Ping::Reply handle(Io::Proto::Ping::Request const& req)
  {
    // TODO: add SW version here
    return { .pong_ = "YALS" };
  }

  Io::Proto::Set_LED_brightness::Reply handle(Io::Proto::Set_LED_brightness::Request const& req)
  {
    auto const b = req.brightness_ / 99.0f;
    ctx_.hal_.led_.brightness(b);
    if( not ctx_.hal_.EEPROM_.LED_brightness(b) )
      return { .err_ = "LED set; EEPROM failed" };
    return {};
  }

  Io::Proto::Set_max_servo_position::Reply handle(Io::Proto::Set_max_servo_position::Request const& req)
  {
    auto const b = req.max_pos_ / 999.0f;
    if(b < ctx_.setpoints_.min_pos_)
      return { .err_ = "below min" };

    ctx_.setpoints_.max_pos_ = b;
    ctx_.setpoints_.position_ = std::min(ctx_.setpoints_.position_, ctx_.setpoints_.max_pos_);
    if( not ctx_.hal_.EEPROM_.max_position(b) )
      return { .err_ = "max set; EEPROM failed" };
    return {};
  }

  Io::Proto::Set_min_servo_position::Reply handle(Io::Proto::Set_min_servo_position::Request const& req)
  {
    auto const b = req.min_pos_ / 999.0f;
    if(ctx_.setpoints_.max_pos_ < b)
      return { .err_ = "above max" };

    ctx_.setpoints_.min_pos_ = b;
    ctx_.setpoints_.position_ = std::max(ctx_.setpoints_.position_, ctx_.setpoints_.min_pos_);
    if( not ctx_.hal_.EEPROM_.min_position(ctx_.setpoints_.min_pos_) )
      return { .err_ = "min set; EEPROM failed" };
    return {};
  }

  Io::Proto::Set_servo_position::Reply handle(Io::Proto::Set_servo_position::Request const& req)
  {
    auto const b = req.pos_ / 999.0f;
    if(b > ctx_.setpoints_.max_pos_)
      return { .err_ = "above max" };
    if(b < ctx_.setpoints_.min_pos_)
      return { .err_ = "below min" };

    ctx_.setpoints_.position_ = b;
    return {};
  }

private:
  Context& ctx_;
};
