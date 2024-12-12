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
  static constexpr unsigned delay_to_write_data_ms = 5; // per datasheet M24C02 needs max 5ms to do a 1B write
  using value_type = uint32_t;

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

  bool write(size_t const address, void const* value, size_t const size)
  {
    auto bytes = static_cast<uint8_t const*>(value);
    for(auto i=0u; i<size; ++i)
      if( not write_byte(address + i, bytes[i]) )
        return false;
    return true;
  }

  bool read(size_t const address, void* value, size_t const size) const
  {
    auto bytes = static_cast<uint8_t*>(value);
    for(auto i=0u; i<size; ++i)
    {
      auto const b = read_byte(address + i);
      if(not b)
        return false;
      bytes[i] = *b;
    }
    return true;
  }

private:
  std::optional<uint8_t> read_byte(uint8_t const addr) const
  {
    if( i2c_write_timeout_us(i2c_dev, i2c_EEPROM_addr, &addr, 1, true,  i2c_byte_timeout_us) != 1 )
      return {};
    uint8_t out{};
    if( i2c_read_timeout_us (i2c_dev, i2c_EEPROM_addr,  &out,  1, false, i2c_byte_timeout_us) != 1 )
      return {};
    return out;
  }

  bool write_byte(uint8_t const addr, uint8_t const byte)
  {
    uint8_t const data[] = { addr, byte };
    auto const r = i2c_write_timeout_us(i2c_dev, i2c_EEPROM_addr, data, sizeof(data), false, i2c_byte_timeout_us);
    if(r != sizeof(data))
      return false;
    sleep_ms(delay_to_write_data_ms);
    return true;
  }

  static constexpr auto pin_i2c_sda = 24;
  static constexpr auto pin_i2c_scl = 25;
  static constexpr auto i2c_dev = i2c0;     // this is derived from pin number + µC datasheet

  static constexpr auto i2c_speed_Hz = 100'000;
  static constexpr auto i2c_byte_timeout_us = 50'000;

  // note that this address does not match what's in the datasheet, yet it works... :shrug:
  static constexpr uint8_t i2c_EEPROM_addr = 0b0101'0000;
};

}
