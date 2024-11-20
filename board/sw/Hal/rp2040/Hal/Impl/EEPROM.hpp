#pragma once
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <optional>
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
  }

  EEPROM(EEPROM const&) = delete;
  EEPROM& operator=(EEPROM const&) = delete;
  EEPROM(EEPROM &&) = delete;
  EEPROM& operator=(EEPROM &&) = delete;

  bool write(size_t const slot, uint32_t value)
  {
    auto const base = slot * sizeof(uint32_t);
    for(auto i=0u; i<sizeof(value); ++i)
    {
      auto const b = ( value & 0xFF000000 ) >> 24;
      if( not write_byte(base + i, b) )
        return false;
      value = value << 8;
    }
    return true;
  }

  std::optional<uint32_t> read(size_t const slot) const
  {
    uint32_t out{};
    auto const base = slot * sizeof(uint32_t);
    for(auto i=0u; i<sizeof(out); ++i)
    {
      auto const b = read_byte(base + i);
      if(not b)
        return {};
      out = (out << 8u) | uint32_t{*b};
    }
    return out;
  }

private:
  std::optional<uint8_t> read_byte(uint8_t const addr) const
  {
    if( i2c_write_timeout_us(i2c_dev, i2c_EEPROM_addr_write, &addr, 1, true,  i2c_byte_timeout_us) != 1 )
      return {};
    uint8_t out{};
    if( i2c_read_timeout_us (i2c_dev, i2c_EEPROM_addr_read,  &out,  1, false, i2c_byte_timeout_us) != 1 )
      return {};
    return out;
  }

  bool write_byte(uint8_t const addr, uint8_t const byte)
  {
    uint8_t const data[] = { addr, byte };
    auto const r = i2c_write_timeout_us(i2c_dev, i2c_EEPROM_addr_write, data, sizeof(data), false, i2c_byte_timeout_us);
    if(r != sizeof(data))
    {
#if 0
      // TODO: why it fails on 2nd write in a row, effectively glitching the chip?!
      // more verbose output for debugging...
      // r==-1 -> timeout
      // r==-2 -> "generic" (address not ACKed)
      char buf[128];
      auto const len = snprintf(buf, sizeof(buf), "WR 0x%02X @ *%d -> res=%d\r\n", byte, addr, r);
      for(auto i=0; i<len; ++i)
      {
        while( not uart_is_writable(uart0) ) { }
        uart_putc(uart0, buf[i]);
      }
#endif
      return false;
    }
    sleep_ms(5);    // M24C02 per datasheet needs max 5ms to do a write
    return true;
  }

  static constexpr auto pin_i2c_sda = 24;
  static constexpr auto pin_i2c_scl = 25;
  static constexpr auto i2c_dev = i2c0;     // this is derived from pin number + µC datasheet

  static constexpr auto i2c_speed_Hz = 100'000;
  static constexpr auto i2c_byte_timeout_us = 50'000;

#if 1
  // NOTE: these addresses do differ compared to the datasheet, yet these do work, while datasheet ones do not. :shrug:
  static constexpr uint8_t i2c_EEPROM_addr_base  = 0b0101'0000;
  static constexpr uint8_t i2c_EEPROM_addr_read  = i2c_EEPROM_addr_base | 0b1000'0000;
  static constexpr uint8_t i2c_EEPROM_addr_write = i2c_EEPROM_addr_base & 0b0111'1111;
  // values from I2C scanner
  static_assert( i2c_EEPROM_addr_read == 0xd0 );
  static_assert( i2c_EEPROM_addr_write== 0x50 );
#else
  // datasheet values
  static constexpr uint8_t i2c_EEPROM_addr_base  = 0b1010'0000;
  static constexpr uint8_t i2c_EEPROM_addr_read  = i2c_EEPROM_addr_base | 0b0000'0001;
  static constexpr uint8_t i2c_EEPROM_addr_write = i2c_EEPROM_addr_base & 0b1111'1110;
#endif
};

}
