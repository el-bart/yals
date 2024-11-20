#include "Hal/EEPROM.hpp"
#include "Hal/Uart.hpp"
#include <cstdio>

void print_help(Hal::Uart& uart)
{
  for(auto str: {
      ">> help:\r\n",
      "h - help\r\n",
  })
  {
    uart.tx(str);
    sleep_ms(10);
  }
}

int main()
{
  Hal::EEPROM eeprom;
  Hal::Uart uart;

  (void)eeprom;         
  uart.tx(">> EEPROM testing app\r\n");

  while(true)
  {
    auto const cmd = uart.rx();
    if(not cmd)
      continue;

    switch(*cmd)
    {
      case 'h': print_help(uart); break;
    }
  }
}
