add_library(hal placeholder.cpp)
target_link_libraries(hal pico_stdlib)
target_link_libraries(hal hardware_adc)
target_link_libraries(hal hardware_pwm)
target_link_libraries(hal hardware_i2c)

function (add_mtest name)
  add_binary("Hal_${name}.mtest" "Hal/${name}.mtest.cpp")
endfunction()

add_mtest(Clock)
add_mtest(EEPROM)
add_mtest(Engine)
add_mtest(Engine_current)
add_mtest(Led)
add_mtest(Position_feedback)
add_mtest(Uart)
add_mtest(Watchdog)
add_mtest(Vcc_feedback)

# some more low-level EEPROM mtests:
add_mtest(EEPROM_impl)
add_mtest(I2C_bus_scan)     # rework of a 3rd-party code
add_mtest(I2C_direct_io)    # AI-generated with some fixes and adjustments
