#pragma once
#include "Context.hpp"
#include "Handler.hpp"

struct Controller final
{
  Controller()
  {
    if( not ctx_.hal_.EEPROM_.marker_check() )
    {
      ctx_.hal_.EEPROM_.min_position(0.0);
      ctx_.hal_.EEPROM_.max_position(1.0);
      ctx_.hal_.EEPROM_.marker_write();
    }

    ctx_.setpoints_.min_pos_ = ctx_.hal_.EEPROM_.min_position();
    ctx_.setpoints_.max_pos_ = ctx_.hal_.EEPROM_.max_position();
    ctx_.setpoints_.position_ = ctx_.hal_.pos_.value();

    if(ctx_.setpoints_.position_ < ctx_.setpoints_.min_pos_)
      ctx_.setpoints_.position_ = ctx_.setpoints_.min_pos_;
    if(ctx_.setpoints_.position_ > ctx_.setpoints_.max_pos_)
      ctx_.setpoints_.position_ = ctx_.setpoints_.max_pos_;
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

  Context ctx_;
  Handler handler_{ctx_};
};
