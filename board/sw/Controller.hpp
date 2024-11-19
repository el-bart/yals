#pragma once
#include "Context.hpp"
#include "Handler.hpp"

struct Controller final
{
  void update()
  {
    // TODO
  }

private:
  Context ctx_;
  Handler handler_{ctx_};
};
