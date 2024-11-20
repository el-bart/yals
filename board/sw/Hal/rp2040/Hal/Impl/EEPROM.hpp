#pragma once
#include "hardware/i2c.h"
#include "pico/stdlib.h"                
#include "pico/binary_info.h"       
#include <cinttypes>
#include <cstdio>

namespace Hal::Impl
{

struct EEPROM
{
  EEPROM()
  {
    gpio_init(pin_i2c_sda);
    gpio_init(pin_i2c_scl);
    gpio_pull_up(pin_i2c_sda);
    gpio_pull_up(pin_i2c_scl);
    gpio_set_function(pin_i2c_sda, GPIO_FUNC_I2C);
    gpio_set_function(pin_i2c_scl, GPIO_FUNC_I2C);
    i2c_init(i2c_dev, i2c_speed_Hz);
    // make I2C pins available to the peripheral
    //bi_decl( bi_2pins_with_func(pin_i2c_sda, pin_i2c_scl, GPIO_FUNC_I2C) );
  }

  EEPROM(EEPROM const&) = delete;
  EEPROM& operator=(EEPROM const&) = delete;
  EEPROM(EEPROM &&) = delete;
  EEPROM& operator=(EEPROM &&) = delete;

  void write(size_t slot, uint32_t value)
  {
    /*
    switch(slot)
    {
      case 0: sim().marker_ = value; return;
      case 1: sim().min_position_ = value / 65535.0f; return;
      case 2: sim().max_position_ = value / 65535.0f; return;
    }
    throw std::runtime_error{"EEPROM::write(): unknown slot: " + std::to_string(slot)};
    */
  }

  uint32_t read(size_t slot) const
  {
    auto const base = slot * sizeof(uint32_t);
    // TODO 4B plz...
    return read_byte(base) / 255.0 * 65535;
    /*
    switch(slot)
    {
      case 0: return sim().marker_;
      case 1: return round( sim().min_position_ * 65535 );
      case 2: return round( sim().max_position_ * 65535 );
    }
    throw std::runtime_error{"EEPROM::read(): unknown slot: " + std::to_string(slot)};
    */
    return 13;  // TODO
  }

private:
  uint8_t read_byte(uint8_t const addr) const
  {
    uint8_t out = 128; //          
    if( i2c_write_timeout_us(i2c_dev, i2c_EEPROM_addr_write, &addr, 1, false, i2c_byte_timeout_us) != 1 )
      return 25;            
    if( i2c_read_timeout_us (i2c_dev, i2c_EEPROM_addr_read,  &out,  1, false, i2c_byte_timeout_us) != 1 )
      return 128;        
    return out;
  }

  void write_byte(uint8_t addr, uint8_t byte)
  {
    // TODO
  }

  static constexpr auto pin_i2c_sda = 24;
  static constexpr auto pin_i2c_scl = 25;
  static constexpr auto i2c_dev = i2c0;     // this is derived from pin number + µC datasheet

  static constexpr auto i2c_speed_Hz = 100'000;
  static constexpr auto i2c_byte_timeout_us = 100'000;

  static constexpr uint8_t i2c_EEPROM_addr_base  = 0b1010'0000;
  static constexpr uint8_t i2c_EEPROM_addr_read  = i2c_EEPROM_addr_base | 0x01;
  static constexpr uint8_t i2c_EEPROM_addr_write = i2c_EEPROM_addr_base & 0xFE;
};

}
