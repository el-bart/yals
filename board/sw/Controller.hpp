#pragma once
#include "Context.hpp"
#include "Handler.hpp"

struct Controller final
{
  Controller()
  {
    ctx_.setpoints_.position_ = ctx_.hal_.pos_.percent();
  }

  void update()
  {
    // TODO
  }

  auto const& context() const { return ctx_; }

private:
  Context ctx_;
  Handler handler_{ctx_};
};
