#pragma once
#include "Context.hpp"
#include "Handler.hpp"
#include <algorithm>

struct Controller final
{
  Controller()
  {
    init_EEPROM();
    init_setpoints();
  }

  void update()
  {
    update_reads();
    // TODO
  }

  auto const& context() const { return ctx_; }

private:
  void update_reads()
  {
    ctx_.last_reads_.vcc_V_ = ctx_.hal_.vcc_.volts();
    ctx_.last_reads_.engine_current_A_ = ctx_.hal_.eng_current_.amps();
    ctx_.last_reads_.position_ = ctx_.hal_.pos_.value();
  }

  bool init_EEPROM()
  {
    auto const mc = ctx_.hal_.EEPROM_.marker_check();
    if(not mc) // EEPROM I/O error...
      return false;

    if(not *mc)
    {
      ctx_.hal_.EEPROM_.min_position(0.0);
      ctx_.hal_.EEPROM_.max_position(1.0);
      ctx_.hal_.EEPROM_.marker_write();
    }
    else
    {
      if(auto const p = ctx_.hal_.EEPROM_.min_position(); p)
        ctx_.setpoints_.min_pos_ = *p;
      if(auto const p = ctx_.hal_.EEPROM_.max_position(); p)
        ctx_.setpoints_.max_pos_ = *p;
    }

    return true;
  }

  void init_setpoints()
  {
    // ekhm... that should never happen, but who'd stop a hacker with a soldering iron... :P
    if(ctx_.setpoints_.max_pos_ < ctx_.setpoints_.min_pos_)
    {
      auto const p = std::clamp(ctx_.setpoints_.min_pos_, 0.0f, 1.0f);
      ctx_.setpoints_.max_pos_ = p;
      ctx_.setpoints_.min_pos_ = p;
    }

    // make sure setpoint for servo is withing min..max range
    ctx_.setpoints_.position_ = ctx_.hal_.pos_.value();
    ctx_.setpoints_.position_ = std::clamp(ctx_.setpoints_.position_, ctx_.setpoints_.min_pos_, ctx_.setpoints_.max_pos_);
  }

  Context ctx_;
  Handler handler_{ctx_};
};
