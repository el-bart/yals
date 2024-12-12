#pragma once
#include "Context.hpp"
#include "Handler.hpp"
#include "Io/Buffer.hpp"
#include "Utils/Engine_controller.hpp"
#include <algorithm>

struct Controller final
{
  Controller():
    eng_ctrl_{ ctx_.hal_.eng_, ctx_.hal_.clock_, ctx_.hal_.pos_.value() }
  {
    init_EEPROM();
    init_setpoints();
    init_LED();
    init_IO();
    init_watchdog();
  }

  void update_only()
  {
    update_reads();
    handle_io();
  }

  void update_and_apply()
  {
    update_only();
    apply_presets();
  }

  auto const& context() const { return ctx_; }

private:
  void update_reads()
  {
    ctx_.last_reads_.vcc_V_ = ctx_.hal_.vcc_.volts();
    ctx_.last_reads_.engine_current_A_ = ctx_.hal_.eng_current_.amps();
    ctx_.last_reads_.position_ = ctx_.hal_.pos_.value();
  }

  void handle_io()
  {
    handle_io_tx();     // free queue space, if there's sth to TX
    handle_io_rx();     // get new data (if present)

    auto const req = extract_line(rx_buffer_);
    if(not req)
      return;
    auto const rep = Io::process(*req, handler_);
    handle_io_tx(rep);
  }

  void handle_io_rx()
  {
    for(auto i=0u; i<Io::Buffer::max_size; ++i)
      if( auto const b = ctx_.hal_.uart_.rx(); b )
        rx_buffer_.dive_add(*b);
      else
        return;
  }

  void handle_io_tx(Io::Line const& line)
  {
    for(auto i=0u; i<line.size_; ++i)
      tx_buffer_.dive_add(line.data_[i]);
    tx_buffer_.dive_add('\n');
    handle_io_tx();
  }

  void handle_io_tx()
  {
    auto sent = 0u;
    for(; sent < tx_buffer_.size_; ++sent)
      if( not ctx_.hal_.uart_.tx( tx_buffer_.data_[sent] ) )
        break;
    tx_buffer_.trim_by(sent);
  }

  void apply_presets()
  {
    ctx_.hal_.led_.brightness(ctx_.setpoints_.LED_brightness_);
    eng_ctrl_.update(ctx_.setpoints_.position_, ctx_.last_reads_.position_);
    reset_watchdog();
  }

  bool init_EEPROM()
  {
    reset_watchdog();
    auto const mc = ctx_.hal_.EEPROM_.marker_check();
    if(not mc) // EEPROM I/O error...
      return false;

    if(*mc)
      read_from_EEPROM();
    else
      persist_in_EEPROM();
    reset_watchdog();

    return true;
  }

  void read_from_EEPROM()
  {
    if(auto const p = ctx_.hal_.EEPROM_.min_position(); p)
      ctx_.setpoints_.min_pos_ = *p;
    if(auto const p = ctx_.hal_.EEPROM_.max_position(); p)
      ctx_.setpoints_.max_pos_ = *p;
    if(auto const b = ctx_.hal_.EEPROM_.LED_brightness(); b)
      ctx_.setpoints_.LED_brightness_ = *b;
  }

  void persist_in_EEPROM()
  {
    reset_watchdog();
    ctx_.hal_.EEPROM_.min_position(ctx_.setpoints_.min_pos_);
    reset_watchdog();
    ctx_.hal_.EEPROM_.max_position(ctx_.setpoints_.max_pos_);
    reset_watchdog();
    ctx_.hal_.EEPROM_.LED_brightness(ctx_.setpoints_.LED_brightness_);
    reset_watchdog();
    ctx_.hal_.EEPROM_.marker_write(); // keep it as the last one
    reset_watchdog();
  }

  bool sanitize(float& value, float const min_value, float const max_value) const
  {
    auto const prev = value;
    value = std::clamp(value, min_value, max_value);
    auto const changes_needed = (prev != value);
    return changes_needed;
  }

  bool sanitize_setpoints()
  {
    auto changes_needed = false;
    changes_needed |= sanitize(ctx_.setpoints_.min_pos_, Utils::Config::servo_absolute_min, Utils::Config::servo_absolute_max);

    // ekhm... that should never happen, but who'd stop a hacker with a soldering iron... :P
    if(ctx_.setpoints_.max_pos_ < ctx_.setpoints_.min_pos_)
    {
      auto const p = std::clamp(ctx_.setpoints_.min_pos_, 0.0f, 1.0f);
      ctx_.setpoints_.max_pos_ = p;
      ctx_.setpoints_.min_pos_ = p;
      //changes_needed = true; TODO!
    }

    // make sure setpoint for servo is within min..max range
    ctx_.setpoints_.position_ = std::clamp(ctx_.hal_.pos_.value(), ctx_.setpoints_.min_pos_, ctx_.setpoints_.max_pos_);

    return changes_needed;
  }

  void init_setpoints()
  {
    if( sanitize_setpoints() )  // were changes needed?
      persist_in_EEPROM();
  }

  void init_LED()
  {
    ctx_.hal_.led_.brightness(ctx_.setpoints_.LED_brightness_);
    ctx_.hal_.led_.set(true);
  }

  void init_IO()
  {
    // purge all input at start, to make sure there's no noise captured during flashing, etc.
    while( ctx_.hal_.uart_.rx() )
      reset_watchdog();
  }

  void init_watchdog()
  {
    if( Hal::Watchdog::rebooted_by_watchdog() )
      tx_watchdog_error();
    reset_watchdog();
  }

  void reset_watchdog() { ctx_.hal_.watchdog_.reset(); }

  void tx_watchdog_error()
  {
    Io::Line line;
    for(auto c: "-ERR: WATCHDOG REBOOT!")
      if(c!=0)
        line.add_byte(c);
    Io::add_checksum(line);
    line.add_byte('\n');

    for(auto i=0u; i<line.size_; ++i)
      while( not ctx_.hal_.uart_.tx(line.data_[i]) )
        reset_watchdog();
  }

  Context ctx_;
  Handler handler_{ctx_};
  Io::Buffer tx_buffer_;
  Io::Buffer rx_buffer_;
  Utils::Engine_controller eng_ctrl_;
};
