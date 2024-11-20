#include "Controller.hpp"

int main()
{
  Controller ctrl;
  // TODO: should be ran at 1kHz
  while(true)
    ctrl.update();
}
