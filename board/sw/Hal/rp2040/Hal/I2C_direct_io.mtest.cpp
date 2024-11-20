#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstdio>

#define I2C_PORT i2c0
#define EEPROM_ADDRESS 0x50  // M24C02 I2C address
#define SDA_PIN 24           // GPIO pin for SDA
#define SCL_PIN 25           // GPIO pin for SCL

// Function to write a byte to EEPROM
void write_eeprom(uint16_t address, uint8_t data)
{
  uint8_t buffer[3];
  buffer[0] = (address >> 8) & 0xFF; // High byte of address
  buffer[1] = address & 0xFF;        // Low byte of address
  buffer[2] = data;                  // Data to write

  if( auto const r = i2c_write_blocking(I2C_PORT, EEPROM_ADDRESS, buffer, sizeof(buffer), false); r != sizeof(buffer) )
    printf("write_eeprom(): ERROR: i2c_write_blocking(): %d\n", r);

  sleep_ms(5); // Wait for the EEPROM to complete the write cycle
}

// Function to read a byte from EEPROM
uint8_t read_eeprom(uint16_t address)
{
  uint8_t buffer[2];
  buffer[0] = (address >> 8) & 0xFF; // High byte of address
  buffer[1] = address & 0xFF;        // Low byte of address

  if( auto const r = i2c_write_blocking(I2C_PORT, EEPROM_ADDRESS, buffer, sizeof(buffer), true); r != sizeof(buffer) )
    printf("read_eeprom(): ERROR: i2c_write_blocking(): %d\n", r);

  uint8_t data;
  if( auto const r = i2c_read_blocking(I2C_PORT, EEPROM_ADDRESS, &data, 1, false); r != 1 )
    printf("read_eeprom(): ERROR: i2c_read_blocking(): %d\n", r);

  return data; // Return the read byte
}


int main()
{
  stdio_init_all(); // Initialize standard I/O
  printf("\ninitializing I2C\n");
  i2c_init(I2C_PORT, 100 * 1000); // Initialize I2C at 100 kHz

  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); // Set SDA pin function
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C); // Set SCL pin function
  gpio_pull_up(SDA_PIN); // Enable pull-up resistor on SDA
  gpio_pull_up(SCL_PIN); // Enable pull-up resistor on SCL

  printf("I2C I/O\n");
  constexpr uint16_t mem_location = 0x0000; // Address to write to

  {
    uint8_t read_data = read_eeprom(mem_location); // Read data back from EEPROM
    printf("initial content: 0x%02X\n", read_data); // Print the read data in hexadecimal format
  }

  {
    uint8_t data_to_write = 0x42;      // Data to write
    write_eeprom(mem_location, data_to_write); // Write data to EEPROM
  }

  {
    uint8_t read_data = read_eeprom(mem_location); // Read data back from EEPROM
    printf("Read Data: 0x%02X\n", read_data); // Print the read data in hexadecimal format
  }

  printf("I2C I/O done\n");
  return 0;
}
